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
#ifndef PLAYLISTUIMODULE_H
#define PLAYLISTUIMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/IPlaylistUI.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/SearchPlaylistGenerationStartedEvent.h"
#include "PlaylistUI.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace Playlist {
            class PlaylistUIModule : public IPlaylistUI {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::Playlist::IPlaylistUI )

                public:
                    PlaylistUIModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "Playlist";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                    virtual void SetupModuleConnections();

                protected:
                    bool event( QEvent *event );
                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent *event );
                    void onFocusedPlaylistItemChanged( RedBullPlayer::Events::FocusedPlayListItemChangedEvent *event );
                    void onFunctionSelected( RedBullPlayer::Events::FunctionSelectedEvent *e );
                    void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent *e );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void onEndOfPlaylist( RedBullPlayer::Events::EndOfPlaylistEvent *event );
                    void onSearchPlaylistGenerationStarted( RedBullPlayer::Events::SearchPlaylistGenerationStartedEvent *e );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    PlaylistUI *_ui;
            };
        }
    }
}

#endif // PLAYLISTUIMODULE_H
