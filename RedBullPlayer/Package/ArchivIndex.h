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
#ifndef ARCHIVINDEX_H
#define ARCHIVINDEX_H

#include <QList>
#include <QString>

// Project includes
#include "ArchivFileReference.h"
#include "ArchivFile.h"


class ArchivIndex {
    public:
        ArchivIndex();
        ArchivIndex( QByteArray indexData );
        void addFileReference( ArchivFileReference* archivFile );
        void removeFileReference( QString fileName );
        void saveIndex( QFile* targetFile );
        QList<ArchivFileReference*>* fileReferences() const;
        ArchivFileReference* getFileReferenceByFileName( QString fileName ) const;

        QByteArray compressedIndexData();

        int fileCount() const;
        int extracedContentSize() const;

    private:
        QDomDocument _index;

    public: // XML NAMES
        static const QString DOCUMENT_ELEMENT;
        static const QString FILE_REFERENCE_ELEMENT;
        static const QString FILENAME_ATTRIBUTE;
        static const QString EXTRACTFILENAME_ATTRIBUTE;
        static const QString OFFSET_ATTRIBUTE;
        static const QString SIZE_ATTRIBUTE;
        static const QString EXTRACTEDSIZE_ATTRIBUTE;
};

#endif // ARCHIVINDEX_H
