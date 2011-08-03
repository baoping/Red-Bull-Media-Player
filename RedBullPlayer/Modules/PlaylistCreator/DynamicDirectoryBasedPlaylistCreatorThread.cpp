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
#include "DynamicDirectoryBasedPlaylistCreatorThread.h"


#include "DummyPlaylist.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/Playlist/PlaylistGenerationFinishedEvent.h"
#include "../../Events/Playlist/StartupPlaylistGenerationStartedEvent.h"
#include "../../Events/Playlist/StartupPlaylistGenerationFinishedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/MediaVanishedEvent.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"

#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

DynamicDirectoryBasedPlaylistCreatorThread::DynamicDirectoryBasedPlaylistCreatorThread(
    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent )
    : QThread( parent ) {
    Q_ASSERT( mediaLibrary != 0 );
    _mediaLibrary = mediaLibrary;
}

DynamicDirectoryBasedPlaylistCreatorThread::~DynamicDirectoryBasedPlaylistCreatorThread() {
    //   _dynDirPlaylistCreator->deleteLater();
}

void DynamicDirectoryBasedPlaylistCreatorThread::run() {
    _dynDirPlaylistCreator = new DynamicDirectoryBasedPlaylistCreator( _mediaLibrary, this );
    connect( _dynDirPlaylistCreator, SIGNAL( playlistCreated( RedBullPlayer::Modules::Playlist::IPlaylist* ) ),
             this, SLOT( playlistCreated( RedBullPlayer::Modules::Playlist::IPlaylist* ) ) );
    connect( _dynDirPlaylistCreator, SIGNAL( mediaDiscovered( QString, int ) ),
             this, SLOT( mediaDiscovered( QString, int ) ) );
    connect( _dynDirPlaylistCreator, SIGNAL( mediaVanished( QString ) ),
             this, SLOT( mediaVanished( QString ) ) );
    connect( _dynDirPlaylistCreator, SIGNAL( mediaRemoved( QString ) ),
             this, SLOT( mediaRemoved( QString ) ) );
    connect( _dynDirPlaylistCreator, SIGNAL( startupPlaylistGenerationStarted() ),
             this, SLOT( startupPlaylistGenerationStarted() ) );
    connect( _dynDirPlaylistCreator, SIGNAL( startupPlaylistGenerationFinished() ),
             this, SLOT( startupPlaylistGenerationFinished() ) );
    connect( _dynDirPlaylistCreator, SIGNAL( playlistGenerationFinished() ),
             this, SLOT( playlistGenerationFinished() ) );
    _dynDirPlaylistCreator->createPlaylistFromBaseDirectory();
    exec();
}

/**
  * Send Events from Thread class connected using SIGNALS/SLOTS. So that the UI receives the events
  * on the correct thread.
  * Can be done here as the Thread class it self stays on the main thread, funny ya!
  */
void DynamicDirectoryBasedPlaylistCreatorThread::playlistCreated( RedBullPlayer::Modules::Playlist::IPlaylist * playlist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Received playlist created" ) );
    PlaylistCreatedEvent e( playlist, PlaylistContentTypeChangedEvent::MEDIA );
    Aggregator::instance()->sendEvent( &e );
}

void DynamicDirectoryBasedPlaylistCreatorThread::mediaDiscovered( QString mediaGuid, int source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Media Discovered" ) );
    QUuid mediaID( mediaGuid );
    MediaDiscoveredEvent::MediaDiscoveredSource mediaSource = MediaDiscoveredEvent::STARTUP_MEDIALIBRARY;

    switch( source ) {
        case 0 : {
                mediaSource = MediaDiscoveredEvent::STARTUP_MEDIALIBRARY;
                break;
            }

        case 1: {
                mediaSource = MediaDiscoveredEvent::STARTUP_FOLDER;
                break;
            }

        case 2 : {
                mediaSource = MediaDiscoveredEvent::USER_ADD;
                break;
            }

        case 3 : {
                mediaSource = MediaDiscoveredEvent::POD_CATCHER;
                break;
            }

        case 4 : {
                mediaSource = MediaDiscoveredEvent::SEARCH;
                break;
            }
    }

    MediaDiscoveredEvent ev( mediaID, mediaSource );
    Aggregator::instance()->sendEvent( &ev );
}

void DynamicDirectoryBasedPlaylistCreatorThread::mediaVanished( QString mediaGuid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Media Vanisheds" ) );
    QUuid mediaID( mediaGuid );
    MediaVanishedEvent ev( mediaID );
    Aggregator::instance()->sendEvent( &ev );
}

void DynamicDirectoryBasedPlaylistCreatorThread::mediaRemoved( QString mediaGuid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Media Removed" ) );
    QUuid mediaID( mediaGuid );
    MediaRemovedEvent ev( mediaID );
    Aggregator::instance()->sendEvent( &ev );
}

void DynamicDirectoryBasedPlaylistCreatorThread::startupPlaylistGenerationStarted() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    StartupPlaylistGenerationStartedEvent ev( new DummyPlaylist() );
    Aggregator::instance()->sendEvent( &ev );
}

void DynamicDirectoryBasedPlaylistCreatorThread::startupPlaylistGenerationFinished() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    StartupPlaylistGenerationFinishedEvent ev;
    Aggregator::instance()->sendEvent( &ev );
}


void DynamicDirectoryBasedPlaylistCreatorThread::playlistGenerationFinished() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Playlist Generation Finished" ) );
    PlaylistGenerationFinishedEvent e;
    Aggregator::instance()->sendEvent( &e );
}
