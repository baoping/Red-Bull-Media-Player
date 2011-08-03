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
#ifndef GRIDVIEWPLAYLISTMODULE_H
#define GRIDVIEWPLAYLISTMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "GridViewPlaylistUI.h"
#include "PlaylistManager.h"

#include "../../Interfaces/IGridViewPlaylist.h"
#include "../../Interfaces/IPlaylistCreator.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/MediaLibrary/CoverImageChangedEvent.h"
#include "../../Events/Player/PlayerExitClickedEvent.h"
#include "../../Events/FocusedPlayListItemChangedEvent.h"
#include "../../Events/Player/PlayerTrackStartedEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/PlaylistClearEvent.h"
#include "../../Events/Playlist/PlaylistGenerationFinishedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/Playlist/SearchPlaylistGenerationStartedEvent.h"
#include "../../Events/Playlist/SearchEndedEvent.h"
#include "../../Events/Playlist/StartupPlaylistGenerationStartedEvent.h"
#include "../../Events/Playlist/StartupPlaylistGenerationFinishedEvent.h"
#include "../../Events/Playlist/CachedPlaylistRestoredEvent.h"
#include "../../Events/Playlist/AddedItemToPlaylistEvent.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"
#include "../../Events/Playlist/MediaVanishedEvent.h"
#include "../../Events/Playlist/RemovedItemFromPlaylistEvent.h"
#include "../../Events/Playlist/DocumentOpenedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Playlist {
            namespace GridViewPlaylist {
                class GridViewPlaylistModule : public IGridViewPlaylist {
                        Q_OBJECT
                        Q_INTERFACES( RedBullPlayer::Modules::Playlist::GridViewPlaylist::IGridViewPlaylist )

                    public:
                        GridViewPlaylistModule();
                        virtual void Setup( QObject* shell, QDomElement config );
                        virtual QString name() const {
                            return "GridViewPlaylist";
                        }
                        virtual QString version() const {
                            return "1.0.0";
                        }
                        virtual void SetupModuleConnections();

                    protected:
                        bool event( QEvent *event );
                        void onCentralUIRegionResized( RedBullPlayer::Events::CentralUIRegionResizedEvent *e );
                        void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent *e );
                        void onNavigationKeyPressed( RedBullPlayer::Events::PlaylistNavigationKeyPressedEvent *e );
                        void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *e );
                        void onCoverImageChanged( RedBullPlayer::Events::CoverImageChangedEvent *e );
                        void onPlayerExitClicked( RedBullPlayer::Events::PlayerExitClickedEvent *e );
                        void onFocusedPlaylistItemChanged( RedBullPlayer::Events::FocusedPlayListItemChangedEvent *e );
                        void onPlayerTrackStarted( RedBullPlayer::Events::PlayerTrackStartedEvent *e );
                        void onMediaItemContentChanged( RedBullPlayer::Events::MediaItemContentChangedEvent *e );
                        void onMediaDiscovered( RedBullPlayer::Events::MediaDiscoveredEvent *e );
                        void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent *e );
                        void onStartupPlaylistGenerationStarted( RedBullPlayer::Events::StartupPlaylistGenerationStartedEvent *e );
                        void onStartupPlaylistGenerationFinished( RedBullPlayer::Events::StartupPlaylistGenerationFinishedEvent *e );
                        void onSearchPlaylistGenerationStarted( RedBullPlayer::Events::SearchPlaylistGenerationStartedEvent *e );
                        void onSearchEnded( RedBullPlayer::Events::SearchEndedEvent *e );
                        void onCachedPlaylistRestored( RedBullPlayer::Events::CachedPlaylistRestoredEvent *e );
                        void onAddedItemToPlaylist( RedBullPlayer::Events::AddedItemToPlaylistEvent *e );
                        void onMediaRemoved( RedBullPlayer::Events::MediaRemovedEvent *e );
                        void onMediaVanished( RedBullPlayer::Events::MediaVanishedEvent *e );
                        void onRemovedItemFromPlaylist( RedBullPlayer::Events::RemovedItemFromPlaylistEvent *e );
                        void onDocumentOpened( RedBullPlayer::Events::DocumentOpenedEvent *e );

                    private:
                        RedBullPlayer::PlayerShell::Shell *_shell;
                        RedBullPlayer::PlayerShell::Region *_region;
                        GridViewPlaylistUI *_ui;
                        RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType _contentType;
                        PlaylistManager *_playlistManager;

                };
            }
        }
    }
}
#endif // GRIDVIEWPLAYLISTMODULE_H
