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
#include "PlaylistManager.h"

#include <QDebug>

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/CachedPlaylistRestoredEvent.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/Playlist/RemovedItemFromPlaylistEvent.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;

PlaylistManager::PlaylistManager() {
    _isInSearchMode = false;
    _isInStartupMode = false;
}

PlaylistManager::~PlaylistManager() {
    if( _currentPlaylist != NULL )
        delete _currentPlaylist;

    if( _cachedPlaylist != NULL )
        delete _cachedPlaylist;
}

void PlaylistManager::setupMediaLibraryConnection() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary *mediaLibrary = qobject_cast<IMediaLibrary*>( plugin );

        if ( mediaLibrary ) {
            _mediaDB = mediaLibrary->mediaDB();
        } else {
            error->setErrorMessage( "Could not cast MediaLibrary Plugin!", Error::Normal );
        }
    } else {
        error->setErrorMessage( "Could not find MediaLibrary Plugin!", Error::Normal );
    }
}

void PlaylistManager::handleMediaDisovered( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "add item with guid : " + mediaId.toString() );

    if( _isInStartupMode ) {
        if( source == MediaDiscoveredEvent::POD_CATCHER ) {
            quequePodcast( mediaId );
            SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "Startup Queque podcast" );
        } else {
            if( source == MediaDiscoveredEvent::STARTUP_MEDIALIBRARY )
                appendItemToCurrentPlaylist( mediaId );

            if( source == MediaDiscoveredEvent::STARTUP_FOLDER )
                prependItemToCurrentPlaylist( mediaId );

            if( source == MediaDiscoveredEvent::STARTUP_MEDIALIBRARY || source == MediaDiscoveredEvent::STARTUP_FOLDER )
                emitAddedItemToPlaylist( mediaId, source );

            SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "Startup Gen" );
        }
    } else {
        if( _isInSearchMode ) {
            if( source == MediaDiscoveredEvent::SEARCH ) {
                appendItemToCurrentPlaylist( mediaId );
                emitAddedItemToPlaylist( mediaId, source );
            } else {
                prependItemToCachedPlaylist( mediaId );
            }

            SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "Search Gen" );
        } else {
            prependItemToCurrentPlaylist( mediaId );
            emitAddedItemToPlaylist( mediaId, source );
            SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "UserAdd|| PodCast Gen" );
        }
    }
}

void PlaylistManager::handleStartupPlaylistGenerationStarted( QPointer<IPlaylist> playlist ) {
    setCurrentPlaylist( playlist );
    _isInStartupMode = true;
}

void PlaylistManager::handleStartupPlaylistGenerationFinished() {
    insertQuequedPodcasts();
    _isInStartupMode = false;
}

void PlaylistManager::handleSearchPlaylistGenerationStarted( QPointer<IPlaylist> playlist ) {
    if( !_isInSearchMode ) {
        cacheCurrentPlaylist();
    } else {
        if( _currentPlaylist != NULL )
            delete _currentPlaylist;
    }

    _isInSearchMode = true;
    setCurrentPlaylist( playlist );
}

void PlaylistManager::handleSearchEnded() {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    restoreCachedPlaylist();
    _isInSearchMode = false;
    emitCachedPlaylistRestored();
}

void PlaylistManager::handleMediaRemoved( QUuid mediaId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "handleMediaRemoved: " + mediaId.toString() );

    if( _currentPlaylist == NULL )
        return;

    int itemIndexCurrentPlaylist = _currentPlaylist->getItemIndexByGuid( mediaId );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "ItemIndex: %1" ).arg( itemIndexCurrentPlaylist ) );

    if( itemIndexCurrentPlaylist >= 0 ) {
        _currentPlaylist->removeItemAt( itemIndexCurrentPlaylist );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "After RemoveAt" ) );
        RemovedItemFromPlaylistEvent e( mediaId );
        Aggregator::instance()->sendEvent( &e );
    }

    if( _cachedPlaylist != NULL && _isInSearchMode ) {
        int itemIndexInCachedPlaylist = _cachedPlaylist->getItemIndexByGuid( mediaId );

        if( itemIndexInCachedPlaylist >= 0 )
            _cachedPlaylist->removeItemAt( itemIndexInCachedPlaylist );
    }
}


void PlaylistManager::handleMediaVanished( QUuid mediaId ) {
    handleMediaRemoved( mediaId );
}

void PlaylistManager::quequePodcast( QUuid mediaId ) {
    _quequedPodcastItems.append( mediaId );
}

void PlaylistManager::insertQuequedPodcasts() {
    if( _quequedPodcastItems.count() == 0 )
        return;

    for( int i = 0; i < _quequedPodcastItems.count(); i++ ) {
        prependItemToCurrentPlaylist( _quequedPodcastItems.at( i ) );
        emitAddedItemToPlaylist( _quequedPodcastItems.at( i ), MediaDiscoveredEvent::POD_CATCHER );
    }

    _quequedPodcastItems.clear();
}

void PlaylistManager::setCurrentPlaylist( QPointer<IPlaylist> playlist ) {
    if( playlist == NULL )
        return;

    _currentPlaylist = playlist;
}

QPointer<IPlaylist> PlaylistManager::getCurrentPlaylist() {
    return _currentPlaylist;
}

void PlaylistManager::cacheCurrentPlaylist() {
    _cachedPlaylist = _currentPlaylist;
}

void PlaylistManager::restoreCachedPlaylist() {
    if( _cachedPlaylist == NULL )
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "_cachedPlaylist is null" );

    _currentPlaylist = _cachedPlaylist;
}

void PlaylistManager::appendItemToCurrentPlaylist( QUuid mediaId ) {
    if( _mediaDB == NULL )
        return;

    if( _currentPlaylist == NULL )
        return;

    IMedia *newItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newItem != NULL ) {
        _currentPlaylist->addItem( newItem );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "appended Item To Playlist Created At: %1" + newItem->createdAt().toString() );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "add Item to Playlist with filename = %1 and title = %2" )
                                         .arg( newItem->fileName() ).arg( newItem->title() ) );
    }
}

void PlaylistManager::prependItemToCurrentPlaylist( QUuid mediaId ) {
    if( _mediaDB == NULL )
        return;

    if( _currentPlaylist == NULL )
        return;

    IMedia *newItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newItem != NULL ) {
        _currentPlaylist->insertItem( 0, newItem );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "prepended Item To Playlist Created At: %1" + newItem->createdAt().toString() );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "add Item to Playlist with filename = %1 and title = %2" )
                                         .arg( newItem->fileName() ).arg( newItem->title() ) );
    }
}

void PlaylistManager::appendItemToCachedPlaylist( QUuid mediaId ) {
    if( _mediaDB == NULL )
        return;

    if( _cachedPlaylist == NULL )
        return;

    IMedia *newItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newItem != NULL ) {
        _cachedPlaylist->addItem( newItem );
    }
}

void PlaylistManager::prependItemToCachedPlaylist( QUuid mediaId ) {
    if( _mediaDB == NULL )
        return;

    if( _cachedPlaylist == NULL )
        return;

    IMedia *newItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newItem != NULL ) {
        _cachedPlaylist->insertItem( 0, newItem );
    }
}

void PlaylistManager::emitAddedItemToPlaylist( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    AddedItemToPlaylistEvent e( mediaId, source );
    Aggregator::instance()->sendEvent( &e );
}

void PlaylistManager::emitCachedPlaylistRestored() {
    CachedPlaylistRestoredEvent e( _currentPlaylist );
    Aggregator::instance()->sendEvent( &e );
}
