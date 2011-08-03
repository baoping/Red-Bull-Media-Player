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
#include "PlayerStateChangedEvent.h"

// Project Includes
#include "../EventTypes.h"

using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::Player;

PlayerStateChangedEvent::PlayerStateChangedEvent( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum state ) : QEvent( PlayerStateChanged ) {
    _state = state;
}

RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum PlayerStateChangedEvent::state() {
    return _state;
}

QString PlayerStateChangedEvent::stateString() {
    QString stateStr = "";

    switch( _state ) {
        case MediaPlayerState::NONE:
            stateStr = "NONE";
            break;

        case MediaPlayerState::OPENING:
            stateStr = "OPENING";
            break;

        case MediaPlayerState::BUFFERING:
            stateStr = "BUFFERING";
            break;

        case MediaPlayerState::PLAYING:
            stateStr = "PLAYING";
            break;

        case MediaPlayerState::PAUSED:
            stateStr = "PAUSED";
            break;

        case MediaPlayerState::STOPPED:
            stateStr = "STOPPED";
            break;

        case MediaPlayerState::ENDED:
            stateStr = "ENDED";
            break;

        case MediaPlayerState::ERROR:
            stateStr = "ERROR";
            break;
    }

    return stateStr;
}
