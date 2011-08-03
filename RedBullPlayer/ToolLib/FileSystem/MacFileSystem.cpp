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
#include "FileSystem.h"


// QT Includes
#include <QDir>
#include <QDebug>

// System, includes
#include <sys/statvfs.h>

using namespace RedBullPlayer::Tools;


bool FileSystem::update() {
    struct statvfs vfs;

    if ( statvfs( _path.toLocal8Bit(), &vfs ) < 0 ) {
        qWarning() << "Could not get free diskspace";
        _freeBytes = _capacaty = _usedBytes = 0;
        return false;
    };

    _freeBytes = vfs.f_bavail * vfs.f_frsize;

    _capacaty = vfs.f_blocks * vfs.f_frsize;

    _usedBytes = _capacaty - _freeBytes;

    return true;
}


