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
#include "filesystemtest.h"


// Qt includes
#include <QDebug>
#include <QString>


// Project
#include "FileSystem/IFileSystem.h"
#include "FileSystem/FileSystem.h"


using namespace RedBullPlayer::Tools;

void FileSystemTest::should_get_diskspace() {
    qWarning() << "CRAPPY TEST!!! Just checks if anything > 0 is returned from OS system call, could not find a way test functinality in cross-os way";
    FileSystem fs( "./" );
    QVERIFY( fs.capacity() > 0 );
    QVERIFY( fs.freeBytes() > 0 );
    QVERIFY( fs.usedBytes() > 0 );
}

