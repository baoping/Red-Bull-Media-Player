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
#ifndef FILESYSTEMFAKE_H
#define FILESYSTEMFAKE_H


#include "FakeLib_global.h"
#include "../../Interfaces/FileSystem/IFileSystem.h"


// Qt includes
#include <qglobal.h>
#include <QString>


// Project Inlcudes



class FAKELIBSHARED_EXPORT FileSystemFake : public RedBullPlayer::Tools::IFileSystem {
    public:
        FileSystemFake( QString path ) {};
        virtual ~FileSystemFake() {}

        virtual quint64 capacity() {
            return _capacity;
        };
        virtual quint64 usedBytes() {
            return _usedBytes;
        };
        virtual quint64 freeBytes() {
            return _freeBytes;
        };

        void setCapacity( quint64 capacity ) {
            _capacity = capacity;
        }
        void setUsedBytes( quint64 usedBytes ) {
            _usedBytes = usedBytes;
        }
        void setFreeBytes( quint64 freeBytes ) {
            _freeBytes = freeBytes;
        }

    private:

        QString _path;
        quint64 _capacity;
        quint64 _usedBytes;
        quint64 _freeBytes;
};

#endif // FILESYSTEMFAKE_H
