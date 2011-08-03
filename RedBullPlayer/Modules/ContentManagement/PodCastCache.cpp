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
#include "PodCastCache.h"

// QT includes
#include <QDebug>
#include <QTimer>
#include <QFileInfo>

//Project includes
#include "../../Container/Constants.h"
#include "../../Interfaces/RssFeed/IRssFeedItem.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../Events/EventTypes.h"
#include "../../Events/RssFeed/RssFeedParseResponseEvent.h"
#include "../../Events/ContentManagement/NewContentAvailableEvent.h"
#include "../../Events/ContentManagement/BulkNewContentAvailableEvent.h"
#include "../../Events/ContentManagement/RemoveContentEvent.h"
#include "../../Events/ContentManagement/AvailableContentCountChangedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"
#include "../../Events/ContentManagement/SkipContentEvent.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

PodCastCache::PodCastCache( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    _rssFeed = 0;
    _availableContent = new QList<IContent*>();
    _downloadedContent = new QList<IContent*>();
    _skippedContent = new QList<IContent*>();
    updateDownloadedContent();
}

PodCastCache::~PodCastCache() {
    _rssFeed = 0;
    _availableContent->clear();
    delete _availableContent;
    _downloadedContent->clear();
    delete _downloadedContent;
    _skippedContent->clear();
    delete _skippedContent;
}

bool PodCastCache::cacheAvailable() {
    return ( _availableContent->count() > 0 );
}

void PodCastCache::updateDownloadedContent() {
    qDebug() << "PodCastCache => UpdateDownloadedContent";
    _downloadedContent->clear();
    int i;
    QList<IContent*>* allContentFinished = _mediaLibrary->mediaDB()->contentRepository()->allContentByState( Constants::CONTENT_DOWNLOAD_FINISHED );

    for( i = 0; i < allContentFinished->count(); i++ ) {
        _downloadedContent->append( allContentFinished->at( i ) );
    }

    if ( _downloadedContent->count() > 0 ) {
        BulkNewContentAvailableEvent ev( _downloadedContent, ContentCategory::DOWNLOADED );
        Aggregator::instance()->sendEvent( &ev );
    }
}

void PodCastCache::updateSkippedContent() {
    qDebug() << "PodCastCache => UpdateSkippedContent";
    _skippedContent->clear();
    int i;
    QList<IContent*>* allSkipped = _mediaLibrary->mediaDB()->contentRepository()->allContentByState( Constants::CONTENT_SKIPPED );
    qDebug() << "PodCastCache => AllSkipped=" << allSkipped->count();

    for( i = 0; i < allSkipped->count(); i++ ) {
        IContent* skippedItem = allSkipped->at( i );

        if ( !isAvailable( skippedItem->guid() ) ) {
            qDebug() << "PodCastCache => SkippedContent is not available -> delete";
            Error err;
            _mediaLibrary->mediaDB()->contentRepository()->deleteContent( *skippedItem, &err );
        } else {
            qDebug() << "PodCastCache => SkippedContent still available -> add to list";
            _skippedContent->append( allSkipped->at( i ) );
        }
    }

    if ( _skippedContent->count() > 0 ) {
        BulkNewContentAvailableEvent ev( _skippedContent, ContentCategory::SKIPPED );
        Aggregator::instance()->sendEvent( &ev );
    }
}

bool PodCastCache::isAvailable( QString contentId ) {
    int i;

    for( i = 0; i < _rssFeed->itemCount(); i++ ) {
        if ( _rssFeed->itemAt( i )->guid() == contentId ) {
            return true;
        }
    }

    return false;
}

void PodCastCache::handleRssFeed( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed ) {
    bool bulkEvent = ( _rssFeed == 0 );
    bool needUpdate = ( bulkEvent || ( rssFeed->pubDate() > _rssFeed->pubDate() ) );

    if ( !needUpdate ) return;

    emit parseStarted();
    int availableContentCount = _availableContent->count();
    _availableContent->clear();
    //Cache old ids for faster parsing
    QList<QString> idCache;

    if ( !bulkEvent ) {
        int y;

        for( y = 0; y < _rssFeed->itemCount(); y++ ) {
            IRssFeedItem* oldItem = _rssFeed->itemAt( y );
            idCache.append( oldItem->guid() );
        }
    }

    int i;

    for( i = 0; i < rssFeed->itemCount(); i++ ) {
        IRssFeedItem* item = rssFeed->itemAt( i );

        if ( idCache.contains( item->guid() ) ) {
            int idx = idCache.indexOf( item->guid() );
            idCache.removeAt( idx );
            continue;
        }

        IContent* content = _mediaLibrary->mediaDB()->contentRepository()->getContentByGuid( item->guid() );

        if ( content ) {
            if ( item->pubDate() <= content->date() ) {
                if ( ( content->state() == Constants::CONTENT_DOWNLOAD_FINISHED ) ||
                        ( content->state() == Constants::CONTENT_SKIPPED ) ||
                        ( content->state() == Constants::CONTENT_BANNED ) ) {
                    continue;
                }
            }
        } else {
            QFileInfo fileInfo( item->enclosure() );
            content = _mediaLibrary->mediaDB()->contentRepository()->createContent( item->guid(),
                      item->title(),
                      fileInfo.fileName(),
                      item->enclosure() );
        }

        content->setFileSize( item->enclosureContentLength().toUInt() );
        qDebug() << "PodCastCache => New Content: " << content->fileName();
        _availableContent->append( content );

        if ( !bulkEvent ) {
            NewContentAvailableEvent ev( content, ContentCategory::NEW );
            Aggregator::instance()->sendEvent( &ev );
        }
    }

    //Delete deprecated items
    for( i = 0; i < idCache.count(); i++ ) {
        RemoveContentEvent ev( idCache.at( i ) );
        Aggregator::instance()->sendEvent( &ev );
    }

    if ( bulkEvent ) {
        BulkNewContentAvailableEvent ev( _availableContent, ContentCategory::NEW );
        Aggregator::instance()->sendEvent( &ev );
    }

    if ( availableContentCount != _availableContent->count() ) {
        AvailableContentCountChangedEvent ev( _availableContent->count() );
        Aggregator::instance()->sendEvent( &ev );
    }

    _rssFeed = rssFeed;
    qDebug() << "PodCastCache => handleRssFeed -> updateSkippedContent";
    updateSkippedContent();
    emit parseFinished();
}



void PodCastCache::setRssFeedImage( QString fileName ) {
    if ( _rssFeed == 0 ) return;

    _rssFeed->setImage( fileName );
}

void PodCastCache::setRssFeedItemImage( QString guid, QImage image ) {
    if ( _rssFeed == 0 ) return;

    int i;

    for( i = 0; i < _rssFeed->itemCount(); i++ ) {
        IRssFeedItem* item = _rssFeed->itemAt( i );

        if ( item->guid() == guid ) {
            item->setRedBullImage( image );
            break;
        }
    }
}

void PodCastCache::setRssFeedItemLocalFileName( QString guid, QString localFileName ) {
    int i;

    for( i = 0; i < _rssFeed->itemCount(); i++ ) {
        if ( _rssFeed->itemAt( i )->guid() == guid ) {
            _rssFeed->itemAt( i )->setLocalFileName( localFileName );
            break;
        }
    }
}

void PodCastCache::saveRssFeedItemToDb( QString guid ) {
    int i;

    for( i = 0; i < _availableContent->count(); i++ ) {
        IContent* item = _availableContent->at( i );

        if ( item->guid() == guid ) {
            saveToMediaDb( item );
            break;
        }
    }
}

void PodCastCache::saveToMediaDb( IContent* content ) {
    qDebug() << "PodCastCache => saveToMediaDb(" << content->guid() << ")";
    bool isUpdate = false;
    IMedia* media = 0;
    IRssFeedItem* rssItem = 0;
    int i;

    for( i = 0; i < _rssFeed->itemCount(); i++ ) {
        if ( _rssFeed->itemAt( i )->guid() == content->guid() ) {
            rssItem = _rssFeed->itemAt( i );
            break;
        }
    }

    qDebug() << "PodCastCache => LocalFileName=" << rssItem->localFileName();
    media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByFileName( rssItem->localFileName() );

    if ( media == 0 ) {
        IMediaType* mediaType = 0;
        QStringList s = rssItem->enclosureContentType().split( "/", QString::SkipEmptyParts );

        if ( s.count() > 0 ) {
            QString type = s.at( 0 );

            if ( type.toLower() == "video" ) {
                mediaType = _mediaLibrary->mediaDB()->videoType();
            } else if ( type.toLower() == "audio" ) {
                mediaType = _mediaLibrary->mediaDB()->audioType();
            } else if ( type.toLower() == "image" ) {
                mediaType = _mediaLibrary->mediaDB()->imageType();
            } else if ( type.toLower() == "application" ) {
                mediaType = _mediaLibrary->mediaDB()->documentType();
            }
        }

        if ( mediaType == 0 ) {
            mediaType = _mediaLibrary->mediaDB()->videoType();
        }

        media = _mediaLibrary->mediaDB()->mediaRepository()->createNewMedia(
                    RedBullPlayer::Tools::UuidCreator::create(),
                    rssItem->localFileName(),
                    *mediaType );
    } else {
        isUpdate = true;
    }

    if ( rssItem->title().isEmpty() || rssItem->title().isNull() ) {
        media->setTitle( QFileInfo( rssItem->localFileName() ).fileName() );
    } else {
        media->setTitle( rssItem->title() );
    }

    media->setFileSize( QFileInfo( rssItem->localFileName() ).size() );
    media->setCopyright( _rssFeed->copyright() );
    media->setUrl( rssItem->enclosure() );
    media->setDescription( rssItem->description() );
    media->setLanguage( _rssFeed->language() );
    media->setDate( rssItem->pubDate() );
    media->setIsNew( true );

    if ( media->mediaType().guid() == _mediaLibrary->mediaDB()->audioType()->guid() ) {
        _mediaLibrary->metadataHandler()->setMetaDataUsingTagLib( media );
    } else {
        media->setArtist( rssItem->author() );
    }

    if ( !rssItem->redBullImageUrl().isEmpty() ) {
        media->setCoverImage( rssItem->redBullImage() );
    } else if ( _rssFeed->imageDescriptor() != 0 ) {
        if ( !_rssFeed->imageDescriptor()->url().isEmpty() ) {
            media->setCoverImage( _rssFeed->image() );
        }
    }

    Error err;
    _mediaLibrary->mediaDB()->mediaRepository()->saveMedia( *media, &err );
    content->setMediaGuid( media->guid() );
    content->setState( Constants::CONTENT_DOWNLOAD_FINISHED );

    if ( _mediaLibrary->mediaDB()->contentRepository()->saveContent( *content, &err ) ) {
        QUuid mediaID = content->mediaGuid();

        if ( !isUpdate ) {
            MediaDiscoveredEvent ev( mediaID, MediaDiscoveredEvent::POD_CATCHER );
            Aggregator::instance()->sendEvent( &ev );
        } else {
            MediaItemContentChangedEvent ev( mediaID );
            Aggregator::instance()->sendEvent( &ev );
        }

        _availableContent->removeAll( content );
        _downloadedContent->append( content );
        RemoveContentEvent removeEvent( content->guid() );
        Aggregator::instance()->sendEvent( &removeEvent );
        NewContentAvailableEvent newContentEvent( content, ContentCategory::DOWNLOADED );
        Aggregator::instance()->sendEvent( &newContentEvent );
        AvailableContentCountChangedEvent ev( _availableContent->count() );
        Aggregator::instance()->sendEvent( &ev );
    }
}

void PodCastCache::skipContent( QString contentId ) {
    IContent* content = 0;
    int i;

    for( i = 0; i < _availableContent->count(); i++ ) {
        IContent* item = _availableContent->at( i );

        if ( item->guid() == contentId ) {
            content = item;
            break;
        }
    }

    if ( content == 0 ) return;

    content->setState( Constants::CONTENT_SKIPPED );
    Error err;

    if ( _mediaLibrary->mediaDB()->contentRepository()->saveContent( *content, &err ) ) {
        _availableContent->removeAll( content );
        _skippedContent->append( content );
        RemoveContentEvent removeEvent( content->guid() );
        Aggregator::instance()->sendEvent( &removeEvent );
        NewContentAvailableEvent newContentEvent( content, ContentCategory::SKIPPED );
        Aggregator::instance()->sendEvent( &newContentEvent );
        AvailableContentCountChangedEvent ev( _availableContent->count() );
        Aggregator::instance()->sendEvent( &ev );
    }
}

void PodCastCache::banContent( QString contentId ) {
    IContent* content = 0;
    int i;

    for( i = 0; i < _skippedContent->count(); i++ ) {
        IContent* item = _skippedContent->at( i );

        if ( item->guid() == contentId ) {
            content = item;
            break;
        }
    }

    if ( content == 0 ) return;

    content->setState( Constants::CONTENT_BANNED );
    Error err;

    if ( _mediaLibrary->mediaDB()->contentRepository()->saveContent( *content, &err ) ) {
        _skippedContent->removeAll( content );
        RemoveContentEvent removeEvent( content->guid() );
        Aggregator::instance()->sendEvent( &removeEvent );
    }
}

void PodCastCache::resetSkipContent( QString contentId ) {
    qDebug() << "PodCastCache => ResetSkipContent(" << contentId << ")";
    IContent* content = 0;
    int i;

    for( i = 0; i < _skippedContent->count(); i++ ) {
        IContent* item = _skippedContent->at( i );

        if ( item->guid() == contentId ) {
            content = item;
            break;
        }
    }

    content->setState( Constants::CONTENT_NEW );
    Error err;

    if ( _mediaLibrary->mediaDB()->contentRepository()->saveContent( *content, &err ) ) {
        _skippedContent->removeAll( content );
        _availableContent->append( content );
        RemoveContentEvent removeEvent( content->guid() );
        Aggregator::instance()->sendEvent( &removeEvent );
        NewContentAvailableEvent newContentEvent( content, ContentCategory::NEW );
        Aggregator::instance()->sendEvent( &newContentEvent );
        AvailableContentCountChangedEvent ev( _availableContent->count() );
        Aggregator::instance()->sendEvent( &ev );
    } else {
        qDebug() << "PodCastCache => ResetSkipContent db erorr=" << err.errorMessage();
    }
}

bool PodCastCache::isSkippedContent( QString contentId ) {
    int i;

    for( i = 0; i < _skippedContent->count(); i++ ) {
        if ( _skippedContent->at( i )->guid() == contentId ) {
            return true;
        }
    }

    return false;
}


ContentDescriptor* PodCastCache::getContentDescriptorById( QString contentId ) {
    ContentDescriptor* descriptor = 0;
    int i;

    for( i = 0; i < _availableContent->count(); i++ ) {
        IContent* item = _availableContent->at( i );

        if ( item->guid() == contentId ) {
            IRssFeedItem* feedItem = 0;

            if ( _rssFeed == 0 )
                qDebug() << "RssFeed = 0";

            int y;

            for( y = 0; _rssFeed->itemCount(); y++ ) {
                if ( _rssFeed->itemAt( y )->guid()  == contentId ) {
                    feedItem = _rssFeed->itemAt( y );
                    break;
                }
            }

            if ( feedItem == 0 ) {
                return 0;
            }

            if ( item == 0 ) {
                qDebug() << "PodCastCache => GetContentDescriptorById content is null?!";
            }

            descriptor = new ContentDescriptor( item, feedItem->redBullImageUrl(), this );
        }
    }

    return descriptor;
}
