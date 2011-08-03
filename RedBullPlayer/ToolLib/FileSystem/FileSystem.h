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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../../Interfaces/FileSystem/IFileSystem.h"

#include <qglobal.h>
#include <QString>

namespace RedBullPlayer {
    namespace Tools {
        class FileSystem : public RedBullPlayer::Tools::IFileSystem {
            public:
                FileSystem( QString path );
                virtual ~FileSystem() {}

                virtual  quint64 capacity();
                virtual quint64 usedBytes();
                virtual  quint64 freeBytes();

            private:
                FileSystem();
                bool update();
                QString _path;
                quint64 _capacaty;
                quint64 _usedBytes;
                quint64 _freeBytes;

        };
    }
}

#endif // FILESYSTEM_H
