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
#include "RequestMetaData.h"

using namespace RedBullPlayer::Modules::SnapshotCreator;

RequestMetaData::RequestMetaData( QUuid id ) {
    _id = id;
    _mediaFileName = "";
    _position = 0.0F;
    _time = 0;
}

RequestMetaData::RequestMetaData( QUuid id, QString mediaFileName, float position ) {
    _id = id;
    _mediaFileName = mediaFileName;
    _position = position;
    _time = 0;
}

RequestMetaData::RequestMetaData( QUuid id, QString mediaFileName, qint64 time ) {
    _id = id;
    _mediaFileName = mediaFileName;
    _position = 0.0F;
    _time = time;
}

RequestMetaData::RequestMetaData( QUuid id, QString mediaFileName, float position, qint64 time ) {
    _id = id;
    _mediaFileName = mediaFileName;
    _position = position;
    _time = time;
}
