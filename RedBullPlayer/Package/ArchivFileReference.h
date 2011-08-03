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
#ifndef ARCHIVFILEREFERENCE_H
#define ARCHIVFILEREFERENCE_H

#include <QString>
#include <QDomDocument>
#include <QDomElement>

class ArchivFileReference {
    public:
        ArchivFileReference( QString fileName, QString extractFileName, qint32 offset, qint32 fileSize, qint32 extractedFileSize );

        void setupXmlElement( QDomElement* element );


        QString fileName() const {
            return _fileName;
        }

        QString extractFileName() const {
            return _extractFileName;
        }
        qint32 offset() const {
            return _offset;
        }
        qint32 fileSize() const {
            return _fileSize;
        }
        qint32 extractedFileSize() const {
            return _extractedFileSize;
        }

        static ArchivFileReference* loadFromXmlElement( const QDomElement& element );

    private:
        ArchivFileReference();

    private:
        QString _fileName;
        QString _extractFileName;
        qint32 _offset;
        qint32 _fileSize;
        qint32 _extractedFileSize;
};

#endif // ARCHIVFILEREFERENCE_H
