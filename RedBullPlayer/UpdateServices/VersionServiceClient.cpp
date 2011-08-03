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
#include "VersionServiceClient.h"

#include <QtCore/QUrl>
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

// Project Includes
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Network;


VersionServiceClient::VersionServiceClient( QUrl serviceUrl, QObject* parent ) : QObject( parent ) {
    Q_ASSERT( serviceUrl.isValid() );
    _networkAccessManager = 0;
    _serviceUrl = serviceUrl;
    bootstrap();
}

VersionServiceClient::~VersionServiceClient() {
    if ( _networkAccessManager != 0 ) {
        delete _networkAccessManager;
    }
}

void VersionServiceClient::bootstrap() {
    _networkAccessManager = new NetworkAccessManager( this );
    connect( _networkAccessManager,
             SIGNAL( finished( QUrl, QByteArray, QString, QString ) ),
             this,
             SLOT( finished( QUrl, QByteArray, QString, QString ) ) );
    connect( _networkAccessManager,
             SIGNAL( failed( QUrl, int, QString ) ),
             this,
             SLOT( failed( QUrl, int, QString ) ) );
}

void VersionServiceClient::checkForUpdate() {
    _networkAccessManager->get( _serviceUrl );
}

void VersionServiceClient::finished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength ) {
    parseVersionServiceResponse( data );
}

void VersionServiceClient::failed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase ) {
    emit noVersionFound(); //in case of error..
}


void VersionServiceClient::parseVersionServiceResponse( QByteArray data ) {
    /*
    <VersionInformationDto xmlns="http://schemas.datacontract.org/2004/07/RedBull.Server.UpdateService.Domain" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
        <DownloadUrl>http://mii.at/someUrl</DownloadUrl>
        <Version>1.1.0.0</Version>
        <ZipSize>100</ZipSize>
        <UnzipSize>1000</UnzipSize>
    </VersionInformationDto>
    */
    QString downloadUrl = "";
    QString serverVersion = "";
    QString zipSize = "0";
    QString unzipSize = "0";
    QXmlStreamReader xml( data );

    while ( !xml.atEnd() && !xml.hasError() ) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if ( token == QXmlStreamReader::StartDocument )
            continue;

        if( token == QXmlStreamReader::StartElement ) {
            if ( xml.name() == "VersionInformationDto" ) {
                xml.readNext();

                while( !( xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "VersionInformationDto" ) ) {
                    if( xml.tokenType() == QXmlStreamReader::StartElement ) {
                        /* DownloadUrl */
                        if( xml.name() == "DownloadUrl" ) {
                            downloadUrl = xml.readElementText();
                        }

                        /* Version */
                        if( xml.name() == "Version" ) {
                            serverVersion = xml.readElementText();
                        }

                        /*ZipSize*/
                        if ( xml.name() == "ZipSize" ) {
                            zipSize = xml.readElementText();
                        }

                        /*UnzipSize*/
                        if ( xml.name() == "UnzipSize" ) {
                            unzipSize = xml.readElementText();
                        }
                    }

                    /* ...and next... */
                    xml.readNext();
                }
            }
        }
    }

    bool ok;
    qulonglong zipSizeValue = zipSize.toULongLong( &ok );

    if ( !ok )
        zipSizeValue = 0;

    qulonglong unzipSizeValue = unzipSize.toULongLong( &ok );

    if ( !ok )
        unzipSizeValue = 0;

    if ( !serverVersion.isEmpty() && !downloadUrl.isEmpty() ) {
        emit versionFound( serverVersion, downloadUrl, zipSizeValue, unzipSizeValue );
    } else {
        emit noVersionFound();
    }
}
