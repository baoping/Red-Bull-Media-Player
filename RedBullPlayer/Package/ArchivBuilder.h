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
#ifndef ARCHIVBUILDER_H
#define ARCHIVBUILDER_H

#include <QFileInfo>
#include <QList>
#include <QTemporaryFile>

// Project includes
#include "ArchivFile.h"
#include "ArchivFileReference.h"
#include "ArchivIndex.h"
#include "ArchivData.h"
#include "ArchivDataReader.h"

class ArchivBuilder {
    public:
        static char DATA_MARKER[];
        static char DATA_MARKER_REPLACECHAR[];
        static uint DATA_MARKER_LENGTH;

    public:
        ArchivBuilder();
        ArchivBuilder( QFileInfo archivFile );
        virtual ~ArchivBuilder();
        void addFile( QString fileName, QString extractFileName, QString newInternalFileName );
        void removeFile( QString fileName );
        QByteArray extractFileData( const ArchivFileReference& fileRef );

        void build( QFileInfo stubFile, QString fileName );
        void build( QByteArray stubData, QString fileName );

        const ArchivIndex& index() const {
            return * _index;
        }

        ArchivDataReader* reader() const {
            return _reader;
        }

    private:
        ArchivData* _data;
        ArchivIndex* _index;
        ArchivDataReader* _reader;
};

#endif // ARCHIVBUILDER_H
