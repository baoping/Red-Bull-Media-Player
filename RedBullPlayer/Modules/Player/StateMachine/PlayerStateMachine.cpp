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
#include "PlayerStateMachine.h"

#include <QList>
#include <QDebug>

//Project includes
#include "../States/PlayerBuffering.h"
#include "../States/PlayerEnded.h"
#include "../States/PlayerError.h"
#include "../States/PlayerOpening.h"
#include "../States/PlayerPaused.h"
#include "../States/PlayerPlaying.h"
#include "../States/PlayerStopped.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Modules::MediaLibrary;

PlayerStateMachine::PlayerStateMachine( PlayerModel* playerModel,
                                        MediaPlayerHandler* mediaPlayerHandler,
                                        IPlayerView* view,
                                        AudioAnimationEngine* audioAnimationEngine,
                                        QObject *parent ):
    QObject( parent ) {
    _mediaPlayerHandler = mediaPlayerHandler;
    _view = view;
    _playerModel = playerModel;
    _audioAnimationEngine = audioAnimationEngine;
    bootstrap();
}

PlayerStateMachine::~PlayerStateMachine() {
    QList<MediaPlayerState::StateEnum> keys = _states.keys();

    for( int i = 0; i < keys.count(); i++ ) {
        IPlayerState* playerState = _states.take( keys.at( i ) );
        playerState->deleteLater();
    }

    _playerModel = 0;
    _mediaPlayerHandler = 0;
    _view = 0;
    _audioAnimationEngine = 0;
}

void PlayerStateMachine::bootstrap() {
    _currentState = MediaPlayerState::NONE;
    _states.insert( MediaPlayerState::BUFFERING, new PlayerBuffering( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::ENDED, new PlayerEnded( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::ERROR, new PlayerError( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::OPENING, new PlayerOpening( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::PAUSED, new PlayerPaused( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::PLAYING, new PlayerPlaying( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
    _states.insert( MediaPlayerState::STOPPED, new PlayerStopped( _playerModel, _mediaPlayerHandler, _view, _audioAnimationEngine, this ) );
}

bool PlayerStateMachine::changeStateTo( MediaPlayerState::StateEnum newState ) {
    if ( _states.contains( _currentState ) ) {
        IPlayerState* currentState = _states.value( _currentState );
        currentState->leaveState();
    }

    if ( _states.contains( newState ) ) {
        IPlayerState* state = _states.value( newState );
        state->enterState();
    }

    _currentState = newState;
    return true;
}
