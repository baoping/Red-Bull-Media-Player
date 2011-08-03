/*
 *  Red Bull Media Player
 *  Copyright (C) 2011, Red Bull
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Collector.h"

//Qt includes
#include <QtCore/QUrl>
#include <QImage>
#include <QFile>
#include <QDebug>
#include <QEventLoop>
#include <QNetworkCookie>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtXml/QXmlStreamReader>
#include <QSysInfo>

//Project includes
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/helper.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::AlbumCoverCollector;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

Collector::Collector( QObject * parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    QString tempDir = ConfigManager::instance()->getTempDirectory();
    resolveMediaLibrary();
    _albumCollector = new QNetworkAccessManager( this );
    connect( _albumCollector, SIGNAL( finished( QNetworkReply* ) ),
             SLOT( slotCollectorFinished( QNetworkReply* ) ) );
    _musicBrainz = new QNetworkAccessManager( this );
    connect( _musicBrainz, SIGNAL( finished( QNetworkReply* ) ),
             SLOT( slotMusicBrainzFinished( QNetworkReply* ) ) );
    _imageDownloader = new DownloadUtil( this );
    _imageDownloader->setLoggerName( Constants::LOGGER_ALBUMCOVERCOLLECTOR );
    _imageDownloader->setTargetDirectory( tempDir );
    connect( _imageDownloader, SIGNAL( downloadFinished( QString, QString, QString ) ),
             SLOT( slotImageDownloadFinished( QString, QString, QString ) ) );
    connect( _imageDownloader, SIGNAL( downloadFailed( QString, int, QString ) ),
             SLOT( slotImageDownloadFailed( QString, int, QString ) ) );
}

void Collector::resolveMediaLibrary() {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast PlaylistCreator Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find PlaylistCreator Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}


void Collector::searchAlbumCover( QUuid mediaGuid, bool allowRequestMetaData ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    _currentRequestMediaGuid = mediaGuid;

    if ( _mediaLibrary->mediaDB()->mediaRepository()->isOnlineContent( _currentRequestMediaGuid ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Invalid request: PodCastItem" );
        emit signalNoImageFound( _currentRequestMediaGuid );
        return;
    }

    IMedia* media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( mediaGuid );
    QString artist = "";
    QString album = "";
    QString title = "";

    if ( media != 0 ) {
        artist = media->artist();
        album = media->album();
        title = media->title();
    }

    QString serviceUrl = "";

    if ( !artist.isEmpty() && !album.isEmpty() ) {
        serviceUrl = QString( "http://sowacs.appspot.com/AWS/webservices.amazon.com/onca/xml?Service=AWSECommerceService&AWSAccessKeyId=%1&Operation=ItemSearch&SearchIndex=Music&Artist=%2&ResponseGroup=Images&Keywords=%3" )
                     .arg( Constants::AMAZON_KEY )
                     .arg( artist )
                     .arg( album );
    } else if ( !album.isEmpty() && !title.isEmpty() ) {
        //call MusicBrainz
        QString musicBrainzGetArtist = QString( "http://musicbrainz.org/ws/1/track/?type=xml&title=%1&release=%2" )
                                       .arg( title )
                                       .arg( album );
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.GetArtist: " + musicBrainzGetArtist );
        _musicBrainz->get( QNetworkRequest( QUrl( musicBrainzGetArtist ) ) );
    } else if ( !artist.isEmpty() && !title.isEmpty() ) {
        //call MusicBrainz
        QString musicBrainzGetArtist = QString( "http://musicbrainz.org/ws/1/track/?type=xml&title=%1&artist=%2" )
                                       .arg( title )
                                       .arg( artist );
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.GetAlbum: " + musicBrainzGetArtist );
        _musicBrainz->get( QNetworkRequest( QUrl( musicBrainzGetArtist ) ) );
     }

    delete media;

    if ( !serviceUrl.isEmpty() ) {
        getAlbumCover( QUrl( serviceUrl ) );
    }
}

void Collector::getAlbumCover( QUrl url ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Collector.Url: " + url.toString() );
    QNetworkRequest request;
    request.setUrl( QUrl( Helper::decodeUrl( url.toString() ) ) );
    _albumCollector->get( request );
}

void Collector::slotMusicBrainzFinished( QNetworkReply * reply ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    int statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    QString httpReasonPhrase = QVariant::fromValue( reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ) ).toString();
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( "MusicBrainz.HttpStatusCode: %1" ).arg( statusCode ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( "MusicBrainz.HttpReasonPhrase: %1" ).arg( httpReasonPhrase ) );
    bool redirected = false;
    QUrl redirectUrl;

    if ( ( statusCode == 301 ) || ( statusCode == 302 ) ) {
        QString location = reply->rawHeader( "Location" );
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.Location: " + location );
        redirected = true;
        redirectUrl = QUrl( location );
    } else if ( statusCode == 200 ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "***** XML *****" );
        QByteArray data = reply->readAll();
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( data ) );
        parseMusicBrainzResponse( data );
    }

    reply->deleteLater();

    if ( redirected ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Collector.Redirect" );
        _musicBrainz->get( QNetworkRequest( redirectUrl ) );
    }
}

void Collector::slotCollectorFinished( QNetworkReply* reply ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    int statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    QString httpReasonPhrase = QVariant::fromValue( reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ) ).toString();
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( "Collector.HttpStatusCode: %1" ).arg( statusCode ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( "Collector.HttpReasonPhrase: %1" ).arg( httpReasonPhrase ) );
    bool redirected = false;
    QUrl redirectUrl;

    if ( ( statusCode == 301 ) || ( statusCode == 302 ) ) {
        QString location = reply->rawHeader( "Location" );
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Collector.Location: " + location );
        redirected = true;
        redirectUrl = QUrl( location );
    } else if ( statusCode == 200 ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "***** XML *****" );
        QByteArray data = reply->readAll();
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, QString( data ) );
        parseAmazonResponse( data );
    } else {
        emit signalNoImageFound( _currentRequestMediaGuid );
    }

    reply->deleteLater();

    if ( redirected ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Collector.Redirect" );
        getAlbumCover( redirectUrl );
    }
}

void Collector::parseAmazonResponse( QByteArray data ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    QXmlStreamReader xml( data );

    if ( xml.hasError() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Amazon.XML.hasError" );
        return;
    }

    QString imageUrl = "";
    bool imageFound = false;

    while ( !xml.atEnd() && !xml.hasError() ) {
        xml.readNext(); //read next token

        //TotalResults
        if ( xml.isStartElement() && xml.name() == "TotalResults" ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Amazon.TotalResults: " + xml.readElementText() );
        }

        if ( xml.isStartElement() && xml.name() == "Item" ) {
            //***** New Item found *****
            SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Amazon.Item found" );

            while ( !( xml.isEndElement() && xml.name() == "Item" ) ) {
                xml.readNext();

                if ( xml.isStartElement() ) {
                    if ( xml.name() == "LargeImage" ) {
                        //Large Image Found
                        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Amazon.LargeImage found" );

                        while ( !( xml.isEndElement() && xml.name() == "LargeImage" ) ) {
                            xml.readNext();

                            if ( xml.isStartElement() ) {
                                if ( xml.name() == "URL" ) {
                                    imageUrl = xml.readElementText();
                                    imageFound = true;
                                    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "Amazon.LargeImageUrl: " + imageUrl );
                                    break;
                                }
                            }
                        }
                    }
                }

                if ( imageFound )
                    break;
            }
        }

        if ( imageFound )
            break;
    }

    //downloadImage
    if ( imageFound && !imageUrl.isEmpty() ) {
        _imageDownloader->downloadFile( QUrl( imageUrl ) );
    }
}

void Collector::parseMusicBrainzResponse( QByteArray data ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    QXmlStreamReader xml( data );

    if ( xml.hasError() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.XML.hasError" );
        return;
    }

    QString artist = "";
    QString album = "";
    bool albumFound = false;
    bool artistFound = false;

    while ( !xml.atEnd() && !xml.hasError() ) {
        xml.readNext(); //read next token

        if ( ( !artistFound ) && ( xml.isStartElement() && xml.name() == "artist" ) ) {
            //***** New Artist found *****
            SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.Artist found" );

            while ( !( xml.isEndElement() && xml.name() == "artist" ) ) {
                xml.readNext();

                if ( xml.isStartElement() ) {
                    if ( xml.name() == "name" ) {
                        //Artist name Found
                        artist = xml.readElementText();
                        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.Artist name: " + artist );
                        artistFound = true;
                    }
                }

                if ( artistFound )
                    break;
            }
        }

        if ( ( !albumFound ) && ( xml.isStartElement() && xml.name() == "release" ) ) {
            //***** New Artist found *****
            SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.Release found" );

            while ( !( xml.isEndElement() && xml.name() == "release" ) ) {
                xml.readNext();

                if ( xml.isStartElement() ) {
                    if ( xml.name() == "title" ) {
                        //Release name Found
                        album = xml.readElementText();
                        SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, "MusicBrainz.Release name: " + album );
                        albumFound = true;
                    }
                }

                if ( albumFound )
                    break;
            }
        }

        if ( artistFound && albumFound )
            break;
    }

    //downloadImage
    if ( artistFound && albumFound ) {
        QString serviceUrl = QString( "http://sowacs.appspot.com/AWS/webservices.amazon.com/onca/xml?Service=AWSECommerceService&AWSAccessKeyId=%1&Operation=ItemSearch&SearchIndex=Music&Artist=%2&ResponseGroup=Images&Keywords=%3" )
                             .arg( Constants::AMAZON_KEY )
                             .arg( artist )
                             .arg( album );
        getAlbumCover( serviceUrl );
    }
}

void Collector::slotImageDownloadFinished( QString fileName, QString contentType, QString contentLength ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    IMedia* media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( _currentRequestMediaGuid );

    if ( media != 0 ) {
        media->setCoverImage( QImage( fileName ) );
        Error err;
        _mediaLibrary->mediaDB()->mediaRepository()->saveMedia( *media, &err );

        if ( QFile::exists( fileName ) )
            QFile::remove( fileName );
    }

    emit signalImageFound( _currentRequestMediaGuid );
}

void Collector::slotImageDownloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    emit signalNoImageFound( _currentRequestMediaGuid );

    if ( QFile::exists( fileName ) )
        QFile::remove( fileName );
}
