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
#include "VLCMediaObject.h"

// QT includes
#include <QDebug>

// Project includes
#include "IMediaMetaData.h"


VLCMediaObject::VLCMediaObject( libvlc_media_t* media, QString mediaLocation ) : IMediaObject() {
    Q_ASSERT( media != 0 );
    _media = libvlc_media_duplicate( media );
    _mediaLocation = mediaLocation;
    _metaData = new VLCMediaMetaData( this );
}

VLCMediaObject::~VLCMediaObject() {
    if ( _media )
        libvlc_media_release( _media );

    delete _metaData;
    _metaData = 0;
}

QString VLCMediaObject::mediaLocation() {
    return _mediaLocation;
}

qint64 VLCMediaObject::duration() {
    libvlc_time_t d = libvlc_media_get_duration ( _media );
    return ( qint64 ) d;
}

IMediaMetaData* VLCMediaObject::metaData() {
    return _metaData;
}

