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
#include "ArchivFileReference.h"

#include "ArchivIndex.h"

ArchivFileReference::ArchivFileReference() {}

ArchivFileReference::ArchivFileReference( QString fileName, QString extractFileName, qint32 offset, qint32 fileSize, qint32 extractedFileSize ) {
    _fileName = fileName;
    _extractFileName = extractFileName;
    _offset = offset;
    _fileSize = fileSize;
    _extractedFileSize = extractedFileSize;
}

void ArchivFileReference::setupXmlElement( QDomElement* element ) {
    element->setAttribute( ArchivIndex::FILENAME_ATTRIBUTE, _fileName );
    element->setAttribute( ArchivIndex::EXTRACTFILENAME_ATTRIBUTE, _extractFileName );
    element->setAttribute( ArchivIndex::OFFSET_ATTRIBUTE, _offset );
    element->setAttribute( ArchivIndex::SIZE_ATTRIBUTE, _fileSize );
    element->setAttribute( ArchivIndex::EXTRACTEDSIZE_ATTRIBUTE, _extractedFileSize );
}

ArchivFileReference* ArchivFileReference::loadFromXmlElement( const QDomElement& element ) {
    ArchivFileReference* ref = new ArchivFileReference();
    ref->_fileName = element.attribute( ArchivIndex::FILENAME_ATTRIBUTE );
    ref->_extractFileName = element.attribute( ArchivIndex::EXTRACTFILENAME_ATTRIBUTE );
    bool ok;
    ref->_offset = element.attribute( ArchivIndex::OFFSET_ATTRIBUTE ).toInt( &ok, 10 );
    Q_ASSERT( ok );
    ref->_fileSize = element.attribute( ArchivIndex::SIZE_ATTRIBUTE ).toInt( &ok, 10 );
    Q_ASSERT( ok );
    ref->_extractedFileSize = element.attribute( ArchivIndex::EXTRACTEDSIZE_ATTRIBUTE ).toInt( &ok, 10 );
    Q_ASSERT( ok );
    return ref;
}
