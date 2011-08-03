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
#ifndef ARCHIVDATAREADER_H
#define ARCHIVDATAREADER_H


// Qt includes
#include <QFileInfo>

// Project includes
#include "ArchivFile.h"
#include "ArchivFileReference.h"

class ArchivDataReader {
    public:
        ArchivDataReader( QFileInfo archiveFileInfo );

        void findSectionOffsets();

        QByteArray compressedFileData( const ArchivFileReference& );
        QByteArray fileData( const ArchivFileReference& );

        qint64 dataSeciotnOffset();
        qint64 indexSectionOffset();

        QByteArray compressedIndexData();
        QByteArray data();
        QByteArray exeStubData();

    private:
        QFileInfo _archiveFileInfo;
        qint64 _dataSeciotnOffset;
        qint64 _indexSectionOffset;
        qint64 _exeStubSize;
};

#endif // ARCHIVDATAREADER_H
