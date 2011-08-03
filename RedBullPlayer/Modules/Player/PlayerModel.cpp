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
#include "PlayerModel.h"

#include <QDebug>

#include "../../Container/Error.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Container;

PlayerModel::PlayerModel( IMediaLibrary* mediaLibrary, QObject* parent ) :
    IMediaModel( mediaLibrary, parent ) {
    _fileName = "";
    _startTime = 0;
}

void PlayerModel::onNewMedia( IMedia* media ) {
    _fileName = media->fileName();
    _startTime = media->startTime();
    _length = media->length();
    _isNew = media->isNew();

    if ( !media->coverImage().isNull() ) {
        _coverImage = QImage( media->coverImage() );
    } else {
        _coverImage = QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL );
    }

    _animationFile = media->startAnimationFile();
    _animationFrameCount = media->animationFrameCount();
    //Check if PodCast
    _isPodCast = mediaLibrary()->mediaDB()->mediaRepository()->isOnlineContent( id() );

    if ( _isPodCast ) {
        _podCastId = mediaLibrary()->mediaDB()->contentRepository()->getContentByMediaGuid( id().toString() )->guid();
    } else {
        _podCastId = "";
    }

    _mediaTypeId = media->mediaType().guid();
}

IMedia* PlayerModel::updateLength( qint64 length ) {
    _length = length;
    IMedia* media = mediaLibrary()->mediaDB()->mediaRepository()->getMediaByGuid( id() ) ;

    if ( media != 0 ) {
        media->setLength( _length );
        Error err;

        if ( !mediaLibrary()->mediaDB()->mediaRepository()->saveMedia( *media, &err ) ) {
            Error::raise( &err, true );
        }
    }

    return media;
}

IMedia* PlayerModel::saveStartTime( qint64 startTime ) {
    _startTime = startTime;
    IMedia* media = mediaLibrary()->mediaDB()->mediaRepository()->getMediaByGuid( id() );

    if ( media != 0 ) {
        qDebug () << "PlayerModel.setStartTime=" << startTime;
        media->setStartTime( startTime );
        Error err;

        if ( !mediaLibrary()->mediaDB()->mediaRepository()->saveMedia( *media, &err ) ) {
            //qError() << "PlayerModel: " << "Could not save media!";
            Error::raise( &err, true );
        }
    }

    return media;
}

void PlayerModel::setIsNew( bool isNew ) {
    _isNew = isNew;
    IMedia* media = mediaLibrary()->mediaDB()->mediaRepository()->getMediaByGuid( id() );

    if ( media != 0 ) {
        media->setIsNew( isNew );
        Error err;

        if ( !mediaLibrary()->mediaDB()->mediaRepository()->saveMedia( *media, &err ) ) {
            Error::raise( &err, true );
        }
    }
}
