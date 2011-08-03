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
#include "PlayerOpening.h"

#include <QPixmap>
#include <QDebug>

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;

PlayerOpening::PlayerOpening( PlayerModel* playerModel,
                              MediaPlayerHandler* mediaPlayerHandler,
                              IPlayerView* view,
                              AudioAnimationEngine* audioAnimationEngine,
                              QObject *parent )
    : IPlayerState( playerModel, mediaPlayerHandler, view, audioAnimationEngine, parent ) {
}

bool PlayerOpening::enterState() {
    if( playerModel()->mediaType() == PlayerModel::AUDIO ) {
        QPixmap cover = QPixmap::fromImage( playerModel()->coverImage() );

        if( cover.isNull() ) {
            cover = QPixmap( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL );
        }

        audioAnimationEngine()->startAnimation( cover );
    } else {
        audioAnimationEngine()->stopAnimations();
    }

    return true;
}

bool PlayerOpening::leaveState() {
    return true;
}
