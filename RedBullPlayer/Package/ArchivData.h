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
#ifndef ARCHIVDATA_H
#define ARCHIVDATA_H

#include <QTemporaryFile>

// Project includes
#include "ArchivFile.h"
#include "ArchivFileReference.h"


class ArchivData {
    public:
        ArchivData();
        ~ArchivData();
        ArchivFileReference* appendFile( ArchivFile* file );
        void removeFile( const ArchivFileReference& reference );
        QByteArray data();
        void setData( QByteArray data );

    private:
        QTemporaryFile* _dataFile;

};

#endif // ARCHIVDATA_H
