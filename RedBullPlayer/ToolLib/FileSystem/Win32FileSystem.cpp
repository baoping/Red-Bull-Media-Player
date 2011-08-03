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
#include <qt_windows.h>
#include <QDir>
#include <QDebug>

using namespace RedBullPlayer::Tools;

bool FileSystem::update() {
    QString sCurDir = QDir::current().absolutePath();
    QDir::setCurrent( _path );
    ULARGE_INTEGER free, total;
    bool bRes = ::GetDiskFreeSpaceExA( 0, &free , &total , NULL );

    if ( !bRes ) return false;

    QDir::setCurrent( sCurDir );
    _freeBytes = static_cast<quint64>( static_cast<__int64>( free.QuadPart ) );
    _capacaty = static_cast<quint64>( static_cast<__int64>( total.QuadPart ) );
    _usedBytes = _capacaty - _freeBytes;
    return true;
}
