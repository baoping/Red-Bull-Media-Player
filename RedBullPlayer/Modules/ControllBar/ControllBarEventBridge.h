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
#ifndef CONTROLLBAREVENTBRIDGE_H
#define CONTROLLBAREVENTBRIDGE_H

#include <QObject>
#include <QKeyEvent>

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/ControllBar/UpdateControllBarModelEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/Player/PlayerVisibilityChangedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../Events/Player/VideoScreenMouseMovedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/ShellWidgetMovedEvent.h"
#include "../../Events/Player/BufferingAnimationChangeVisibilityEvent.h"
#include "../../Events/SnapshotCaptureEnabledEvent.h"
#include "../../Events/Player/FullscreenStatusChangedEvent.h"
#include "../../Events/Player/PlayerBackwardSpeedChangedEvent.h"
#include "../../Events/Player/PlayerForwardSpeedChangedEvent.h"
#include "../../Events/Player/PlayerVolumeChangedEvent.h"
#include "../../Events/Player/PlayerExitClickedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ControllBar {
            class ControllBarEventBridge : public QObject {
                    Q_OBJECT

                public:
                    explicit ControllBarEventBridge( QObject *parent = 0 );
                    ~ControllBarEventBridge();

                    void controllBarVisibiltyChanged( bool visible );
                    void playClicked();
                    void pauseClicked();
                    void backwardClicked();
                    void forwardClicked();
                    void previewClicked();
                    void fullScreenClicked();
                    void snapshotClicked();
                    void muteClicked( bool mute );
                    void resolutionClicked();

                protected:
                    bool event( QEvent* event );

                    //ControllBar
                    void onUpdateControllBarModel( RedBullPlayer::Events::UpdateControllBarModelEvent* e );

                    //MediaPlayer
                    void onPlayerTimeChanged( RedBullPlayer::Events::PlayerTimeChangedEvent* e );
                    void onPlayerStateChanged( RedBullPlayer::Events::PlayerStateChangedEvent* e );
                    void onPlayerVisibilityChanged( RedBullPlayer::Events::PlayerVisibilityChangedEvent* e );
                    void onSnapshotCaptureEnabled( RedBullPlayer::Events::SnapshotCaptureEnabledEvent* e );
                    void onPlayerForwardSpeedChanged( RedBullPlayer::Events::PlayerForwardSpeedChangedEvent* e );
                    void onPlayerBackwardSpeedChanged( RedBullPlayer::Events::PlayerBackwardSpeedChangedEvent* e );
                    void onPlayerVolumneChanged( RedBullPlayer::Events::PlayerVolumeChangedEvent* e );

                    //PlayList
                    void onPlayListItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );

                    //CentralUI
                    void onCentralUIRegionResized( RedBullPlayer::Events::CentralUIRegionResizedEvent* e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent* e );

                    //PlayerView
                    void onVideoScreenMouseMoved( RedBullPlayer::Events::VideoScreenMouseMovedEvent* e );

                    //Buffering
                    void onBufferingAnimationChangeVisibility( RedBullPlayer::Events::BufferingAnimationChangeVisibilityEvent* e );

                    //Common
                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent* e );
                    void onFullscreenStatusChanged( RedBullPlayer::Events::FullscreenStatusChangedEvent* e );

                    // Player Exit
                    void onPlayerExitClicked( RedBullPlayer::Events::PlayerExitClickedEvent* e );

                signals:
                    void updateControllBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media, bool skipDocuments );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    void centralUIRegionResized( int width, int height );
                    void shellWidgetMoved();
                    void playerVisibilityChanged( bool visible );
                    void videoScreenMouseMoved();
                    void keyPressed( QKeyEvent* keyEvent );
                    void bufferingAnimationChangedVisibility( bool visible );
                    void snapshotCaptureEnabled( bool enable );
                    void fullScreenStatusChanged( bool isFullScreen );
                    void playerForwardSpeedChanged( int speed );
                    void playerBackwardSpeedChanged( int speed );
                    void playerVolumeChanged( int volumne );
                    void playerExitClicked();
            };
        }
    }
}

#endif // CONTROLLBAREVENTBRIDGE_H
