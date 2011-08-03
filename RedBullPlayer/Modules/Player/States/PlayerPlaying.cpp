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
#include "PlayerPlaying.h"

#include <QDebug>

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;

PlayerPlaying::PlayerPlaying( PlayerModel* playerModel,
                              MediaPlayerHandler* mediaPlayerHandler,
                              IPlayerView* view,
                              AudioAnimationEngine* audioAnimationEngine,
                              QObject *parent )
    : IPlayerState( playerModel, mediaPlayerHandler, view, audioAnimationEngine, parent ) {
}

bool PlayerPlaying::enterState() {
    if( playerModel()->mediaType() == PlayerModel::AUDIO ) {
        audioAnimationEngine()->unpauseAnimation();
    }

    mediaPlayerHandler()->setBackwardSpeed( 1, false );
    mediaPlayerHandler()->setForwardSpeed( 1, false );

    if ( mediaPlayerHandler()->timeOnNextTick() ) {
        mediaPlayerHandler()->setTime( playerModel()->startTime() );
        mediaPlayerHandler()->setTimeOnNextTick( false );
    }

    return true;
}

bool PlayerPlaying::leaveState() {
    return true;
}
