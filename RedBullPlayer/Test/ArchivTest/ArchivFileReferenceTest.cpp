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
#include "ArchivFileReferenceTest.h"

// Qt includes
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

// Project includes
#include "../../../RedBullPlayer/Package/ArchivIndex.h"
#include "../../../RedBullPlayer/Package/ArchivFileReference.h"

void ArchivFileReferenceTest::should_setup_xml_element() {
    QDomDocument doc;
    ArchivFileReference ref( "fileName", "extractFileName", 10, 20, 100 );
    QDomElement elem = doc.createElement( ArchivIndex::FILE_REFERENCE_ELEMENT );
    ref.setupXmlElement( &elem );
    QVERIFY( elem.attribute( ArchivIndex::FILENAME_ATTRIBUTE ) == "fileName" );
    QVERIFY( elem.attribute( ArchivIndex::EXTRACTFILENAME_ATTRIBUTE ) == "extractFileName" );
    QVERIFY( elem.attribute( ArchivIndex::OFFSET_ATTRIBUTE ) == "10" );
    QVERIFY( elem.attribute( ArchivIndex::SIZE_ATTRIBUTE ) == "20" );
    QVERIFY( elem.attribute( ArchivIndex::EXTRACTEDSIZE_ATTRIBUTE ) == "100" );
}


void ArchivFileReferenceTest::should_load_from_xml_element() {
    QDomDocument doc;
    ArchivFileReference ref( "fileName", "extractFileName", 10, 20, 100 );
    QDomElement elem = doc.createElement( ArchivIndex::FILE_REFERENCE_ELEMENT );
    ref.setupXmlElement( &elem );
    ArchivFileReference* parsed = ArchivFileReference::loadFromXmlElement( elem );
    QVERIFY( parsed->fileName() == "fileName" );
    QVERIFY( parsed->extractFileName() == "extractFileName" );
    QVERIFY( parsed->offset() == 10 );
    QVERIFY( parsed->fileSize() == 20 );
    QVERIFY( parsed->extractedFileSize() == 100 );
}


