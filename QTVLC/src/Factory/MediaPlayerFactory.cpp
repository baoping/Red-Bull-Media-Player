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
#include "MediaPlayerFactory.h"

// QT includes
#include <QObject>
#include <QDebug>

// Project includes
#include "IMediaPlayer.h"
#include "VLCMediaPlayer.h"
#include "Exception.h"

MediaPlayerFactory* MediaPlayerFactory::_instance = 0;

MediaPlayerFactory::MediaPlayerFactory() {
    _playbackInstance = new VLCInstance( MediaPlayerPurpose::PLAY_BACK );
    _snapshotInstance = new VLCInstance( MediaPlayerPurpose::SNAPSHOT_CREATION );
}

MediaPlayerFactory::~MediaPlayerFactory() {
    delete _playbackInstance;
    delete _snapshotInstance;
}

void MediaPlayerFactory::disposeFactory() {
    if ( _instance != 0 ) {
        delete _instance;
        _instance = 0;
    }
}

MediaPlayerFactory* MediaPlayerFactory::instance() {
    if ( _instance == 0 )
        _instance = new MediaPlayerFactory();

    Q_ASSERT( _instance != 0 );
    return _instance;
}


IMediaPlayer* MediaPlayerFactory::createMediaPlayer( MediaPlayerPurpose::Purpose purpose, QObject* parent ) {
    if ( purpose == MediaPlayerPurpose::PLAY_BACK ) {
        return new VLCMediaPlayer( _playbackInstance, parent );
    } else if ( purpose == MediaPlayerPurpose::SNAPSHOT_CREATION ) {
        return new VLCMediaPlayer( _snapshotInstance, parent );
    }

    return 0;
}

IMediaPlayer* MediaPlayerFactory::createMediaPlayerForSnapshot( QObject* parent ) {
    return new VLCMediaPlayer( _snapshotInstance, parent );
}

