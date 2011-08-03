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
#ifndef DIRECTORYPACKER_H
#define DIRECTORYPACKER_H

// QT includes
#include <QFileInfo>
#include <QDir>
#include <QString>

// Project includes
#include "Package.h"

class DirectoryPacker {
    public:
        DirectoryPacker( Package* pkg, QFileInfo directory );
        ~DirectoryPacker();

        void pack( QString extractBaseDir );

    private:
        void generateFileList( QFileInfo currDir, QStringList* fileList );

    private:
        QDir _dirctory;
        Package* _package;
};

#endif // DIRECTORYPACKER_H
