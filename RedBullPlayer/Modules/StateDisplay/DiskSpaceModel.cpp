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
#include "DiskSpaceModel.h"

#include "../../ToolLib/FileSystem/StorageFactory.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Tools;

DiskSpaceModel::DiskSpaceModel( QObject *parent ) :
    IDiskSpaceModel( parent ) {
}

DiskSpaceModel::~DiskSpaceModel() {
}

QString DiskSpaceModel::freeSpaceString() {
    return convertBytesToHumanReadableString( StorageFactory::defaultStorage()->freeBytes() );
}

QString DiskSpaceModel::maximumSpaceString() {
    return convertBytesToHumanReadableString( StorageFactory::defaultStorage()->capacity() );
}

QString DiskSpaceModel::convertBytesToHumanReadableString( quint64 bytes ) {
    float megaBytes = bytes / 1048576;

    if ( megaBytes >= 1024 ) {
        float gigaBytes = megaBytes / 1024;
        QString gigaBytesString = "";
        return QString( "%1 GB" ).arg( gigaBytesString.setNum( gigaBytes, 'f', 2 ) );
    }

    QString megaBytesString = "";
    return QString( "%1 MB" ).arg( megaBytesString.setNum( megaBytes, 'f', 2 ) );
}

