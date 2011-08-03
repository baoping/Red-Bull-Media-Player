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
#ifndef MEDIAPLAYERFACTORY_H
#define MEDIAPLAYERFACTORY_H

#include <QUrl>

#include "../Interfaces/MediaPlayerPurpose.h"

// QT Forwards
class QObject;
class VLCInstance;

// Project Forwards
class IMediaPlayer;

class MediaPlayerFactory {
    public:
        IMediaPlayer* createMediaPlayer( MediaPlayerPurpose::Purpose purpose, QObject* parent = 0 );
        IMediaPlayer* createMediaPlayerForSnapshot( QObject* parent = 0 );
        static MediaPlayerFactory* instance();
        static void disposeFactory();

    private:
        MediaPlayerFactory();
        ~MediaPlayerFactory();

        VLCInstance* _playbackInstance;
        VLCInstance* _snapshotInstance;

        static MediaPlayerFactory* _instance;
};

#endif // MEDIAPLAYERFACTORY_H
