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
#ifndef PLAYEREVENTBRIDGE_H
#define PLAYEREVENTBRIDGE_H

#include <QObject>
#include <QSize>
#include <QUuid>
#include <QKeyEvent>

//Project includes
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/Player/PlayerRequestTimeChangeEvent.h"
#include "../../Events/Player/PlayerVolumeChangedEvent.h"
#include "../../Events/Player/PlayerCloseButtonClickedEvent.h"
#include "../../Events/Player/PlayerExitClickedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../Events/ShellWidgetMovedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class PlayerEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit PlayerEventBridge( QObject *parent = 0 );
                    ~PlayerEventBridge();

                    void changePlayerVisibility( bool visible );
                    void changeCentralFocus( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType centralWidgetType );
                    void playerPositionChanged( float position );
                    void playerTimeChanged( qint64 time );
                    void mouseMovedOnPlayerView();
                    void fullScreenChanged( bool isFullScreen );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );

                    void forwardSpeedChanged( int forwardSpeed );
                    void backwardSpeedChanged( int backwardSpeed );

                    void playerTrackStarted( QUuid mediaId, QUuid mediaTypeId );

                    void changeBufferingAnimationVisibility( bool visible );
                    void documentOpened( QUuid mediaId );

                protected:
                    bool event( QEvent* event );

                    void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent* e );
                    void onPlayerTimeChanged( RedBullPlayer::Events::PlayerTimeChangedEvent* e );
                    void onPlayerRequestTimeChange( RedBullPlayer::Events::PlayerRequestTimeChangeEvent* e );
                    void onPlayerVolumeChangedEvent( RedBullPlayer::Events::PlayerVolumeChangedEvent* e );
                    void onPlayerCloseButtonClicked( RedBullPlayer::Events::PlayerCloseButtonClickedEvent* e );
                    void onPlayerExitClicked( RedBullPlayer::Events::PlayerExitClickedEvent* e );

                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent* e );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent* e );

                    void onFunctionSelected( RedBullPlayer::Events::FunctionSelectedEvent* e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent *e );
                signals:
                    void centralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType centralWidgetType );
                    void requestTimeChange( qint64 time );
                    void playerVolumeChanged( int volume );
                    void playerCloseButtonClicked();
                    void playerExitClicked();
                    void keyPressed( QKeyEvent* keyEvent );
                    void mainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion uiRegion );
                    void functionSelected( QString functionTitle );

                public slots:

            };
        }
    }
}

#endif // PLAYEREVENTBRIDGE_H
