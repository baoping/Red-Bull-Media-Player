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
#include "ImageListHandler.h"

#include <QFile>

using namespace RedBullPlayer::Widgets;

ImageListHandler::ImageListHandler() {
    _images = new QMap<ButtonImageType, QPixmap*>();
    _imageSize = QSize( 0, 0 );
}

ImageListHandler::~ImageListHandler() {
    if ( !_images->isEmpty() ) {
        if ( _images->contains( RedBullPlayer::Widgets::NORMAL ) ) {
            delete _images->take( RedBullPlayer::Widgets::NORMAL );
        }

        if ( _images->contains( RedBullPlayer::Widgets::MOUSEOVER ) ) {
            delete _images->take( RedBullPlayer::Widgets::MOUSEOVER );
        }

        if ( _images->contains( RedBullPlayer::Widgets::DISABLED ) ) {
            delete _images->take( RedBullPlayer::Widgets::DISABLED );
        }
    }

    delete _images;
}

void ImageListHandler::addImage( ButtonImageType imageType, QString imageUrl ) {
    if ( QFile::exists( imageUrl ) ) {
        QPixmap* _image;

        if ( _images->contains( imageType ) ) {
            _image = _images->take( imageType );
            delete _image;
        }

        _image = new QPixmap( imageUrl );

        if ( imageType == RedBullPlayer::Widgets::NORMAL ) {
            _imageSize = QSize( _image->width(), _image->height() );
        }

        _images->insert( imageType, _image );
    }
}

QPixmap ImageListHandler::getImage( ButtonImageType imageType ) {
    if ( _images->contains( imageType ) ) {
        return *_images->value( imageType );
    } else if ( ( imageType == RedBullPlayer::Widgets::MOUSEOVER ) && ( _images->contains( RedBullPlayer::Widgets::CHECKED ) ) ) {
        return *_images->value( RedBullPlayer::Widgets::CHECKED );
    } else if ( _images->contains( RedBullPlayer::Widgets::NORMAL ) ) {
        return *_images->value( RedBullPlayer::Widgets::NORMAL ); //use normal as default image
    }

    return 0; //replace with placeholder from imagefactory?
}
