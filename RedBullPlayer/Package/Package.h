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
#ifndef PACKAGE_H
#define PACKAGE_H

// QT includes
#include <QObject>
#include <QFileInfo>

// Project includes
#include "ArchivBuilder.h"
class Package : public QObject {
        Q_OBJECT
    public:
        virtual ~Package();
        enum OpenMode {
            Create = 1,
            Edit = 2
        };

        static Package* openCreate( QFileInfo exeStub );
        static Package* openEdit( QFileInfo existingPackage );


        void addFile( QString fileName, QString extractFileName, QString newInternalFileName = QString::null );
        void removeFile( QString fileName );
        void extractAll( QString basePath = QString::null );
        void extractFile( QString fileName, QString toFile );
        void enshurePathForFile( QFileInfo fileName );
        void writeFileTo( QByteArray data, QFileInfo extractFileInfo );
        void save( QString fileName );
        int requiredDiskSpace();

    private:
        Package();
        QFileInfo _exeStub;
        QFileInfo _existingPackage;
        ArchivBuilder* _archivBuilder;
        OpenMode _openMode;

    signals:
        void startExtracting( int fileCount );
        void extractingFile( QString fileName );
        void skippingFile( QString fileName );
        void checkingFile( QString fileName );
        void finishExtracting();
};

#endif // PACKAGE_H
