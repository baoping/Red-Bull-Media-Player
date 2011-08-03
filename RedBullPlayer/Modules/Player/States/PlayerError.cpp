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
#include "PlayerError.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;

PlayerError::PlayerError( PlayerModel* playerModel,
                          MediaPlayerHandler* mediaPlayerHandler,
                          IPlayerView* view,
                          AudioAnimationEngine* audioAnimationEngine,
                          QObject *parent )
    : IPlayerState( playerModel, mediaPlayerHandler, view, audioAnimationEngine, parent ) {
}

bool PlayerError::enterState() {
    mediaPlayerHandler()->setBackwardSpeed( 1, false );
    mediaPlayerHandler()->setForwardSpeed( 1, false );
    return true;
}

bool PlayerError::leaveState() {
    return true;
}
