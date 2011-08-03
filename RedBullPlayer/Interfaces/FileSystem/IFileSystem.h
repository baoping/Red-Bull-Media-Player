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
#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "qglobal.h"

namespace RedBullPlayer {
    namespace Tools {
        /**
         * Class to abstract a filesystem
         */
        class IFileSystem {
            public:
                virtual ~IFileSystem() {}
                /**
                 * \returns the capacity of the filesystem
                 */
                virtual quint64 capacity() = 0;

                /**
                 * \returns The number of already used bytes on the filesystem
                 */
                virtual quint64 usedBytes() = 0;

                /**
                  * \returns The number of available bytes to the currebnt user
                  */
                virtual quint64 freeBytes() = 0;
        };
    }
}

#endif // IFILESYSTEM_H
