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
#include "PlaylistCreatorModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QDir>

// Project Includes
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/Playlist/PlaylistClearEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

#include "../MediaLibrary/Media.h"
#include "../MediaLibrary/MediaType.h"

#include "DummyPlaylistItem.h"
#include "DummyChapter.h"
#include "DummyPlaylist.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;

PlaylistCreatorModule::PlaylistCreatorModule() : IPlaylistCreator() {
    _mediaLibrary = 0;
}

void PlaylistCreatorModule::Setup( QObject* /*shell*/, QDomElement /*config*/ ) {
}

void PlaylistCreatorModule::SetupModuleConnections() {
    resolveMediaLibrary();
    Q_ASSERT( _mediaLibrary != 0 );
    _dynDirPlaylistCreatorThread = new DynamicDirectoryBasedPlaylistCreatorThread( _mediaLibrary, this );
    _tvPlaylistCreator = new TVPlaylistCreator( _mediaLibrary, this );
    _aboutPlaylistCreator = new AboutPlaylistCreator( this );
    _searchDrivenPlaylistCreator = new SearchDrivenPlaylistCreator( _mediaLibrary, this );
    _playlistItemEventHandler = new PlaylistItemEventHandler( _mediaLibrary, this );
    _newsPlaylistCreator = new NewsPlaylistCreator( _mediaLibrary, this );
    _settingsPlaylistCreator = new SettingsPlaylistCreator( this );
    _settingsEventHandler = new SettingsEventHandler( _mediaLibrary, this );
    emitPlaylistClear();
    _dynDirPlaylistCreatorThread->start( QThread::NormalPriority );
}

void PlaylistCreatorModule::resolveMediaLibrary() {
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

void PlaylistCreatorModule::emitPlaylistClear() {
    IPlaylist *emptyPlaylist = new DummyPlaylist();
    PlaylistClearEvent e( emptyPlaylist );
    Aggregator::instance()->sendEvent( &e );
}

Q_EXPORT_PLUGIN2( PlaylistCreatorModule, RedBullPlayer::Modules::PlaylistCreator::PlaylistCreatorModule )
