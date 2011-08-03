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
#ifndef SNAPSHOTHELPER_H
#define SNAPSHOTHELPER_H

//Qt Includes
#include <QObject>

//Project Includes
#include "MediaPlayerHandler.h"
#include "../../../Events/Player/PlayerStateChangedEvent.h"
#include "../PlayerModel.h"

class SnapshotHelper : public QObject {
        Q_OBJECT
    public:
        SnapshotHelper( RedBullPlayer::Modules::Player::PlayerModel* playerModel,
                        MediaPlayerHandler* mediaPlayerHandler );
        ~SnapshotHelper();

        static bool setAllPermissions( QString fileName );

    public slots:
        void takeSnapshot( RedBullPlayer::Container::Error *error );
        void takeSnapshot( QString fileName, RedBullPlayer::Container::Error *error, unsigned int width = 0, unsigned int height = 0 );

    protected:
        bool event ( QEvent * e );
        void onPlayerStateChanged( RedBullPlayer::Events::PlayerStateChangedEvent *e );

    private:
        RedBullPlayer::Modules::Player::PlayerModel* _playerModel;
        MediaPlayerHandler *_mediaPlayerHandler;
};

#endif // SNAPSHOTHELPER_H
