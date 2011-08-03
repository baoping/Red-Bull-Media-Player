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
#include "IMediaModel.h"

using namespace RedBullPlayer::Modules::Player;

IMediaModel::IMediaModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent )
    : QObject( parent ) {
    _hasMedia = false;
    _isDocument = false;
    _mediaType = IMediaModel::UNKOWN;
    _mediaLibrary = mediaLibrary;
}

IMediaModel::MediaTypeEnum IMediaModel::resolveMediaType( RedBullPlayer::Modules::MediaLibrary::IMedia* media ) {
    if ( media->mediaType().guid() == _mediaLibrary->mediaDB()->audioType()->guid() ) {
        return IMediaModel::AUDIO;
    }

    if ( media->mediaType().guid() == _mediaLibrary->mediaDB()->imageType()->guid() ) {
        return IMediaModel::IMAGE;
    }

    if ( media->mediaType().guid() == _mediaLibrary->mediaDB()->videoType()->guid() ) {
        return IMediaModel::VIDEO;
    }

    if ( media->mediaType().guid() == _mediaLibrary->mediaDB()->tvChannelType()->guid() ) {
        return IMediaModel::STREAM;
    }

    if( media->mediaType().guid() == mediaLibrary()->mediaDB()->documentType()->guid() ) {
        return IMediaModel::DOCUMENT;
    }

    return IMediaModel::UNKOWN;
}

QString IMediaModel::mediaTypeString( MediaTypeEnum mediaType ) {
    switch( mediaType ) {
        case IMediaModel::UNKOWN:
            return "UNKOWN";

        case IMediaModel::VIDEO:
            return "VIDEO";

        case IMediaModel::AUDIO:
            return "AUDIO";

        case IMediaModel::IMAGE:
            return "IMAGE";

        case IMediaModel::STREAM:
            return "STREAM";

        case IMediaModel::DOCUMENT:
            return "DOCUMENT";

        default:
            return "";
    }
}

void IMediaModel::setMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media ) {
    _id = media->guid();
    _mediaType = resolveMediaType( media );
    _isDocument = ( _mediaType == IMediaModel::DOCUMENT );
    onNewMedia( media );
    _hasMedia = ( _mediaType != IMediaModel::UNKOWN );
    emit mediaChanged();
}
