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
#include "ArchivIndexTest.h"


#include <QTemporaryFile>
#include <QFile>

// Project includes
#include "../../../RedBullPlayer/Package/ArchivIndex.h"
#include "../../../RedBullPlayer/Package/ArchivFileReference.h"
void ArchivIndexTest::should_save_to_file() {
    ArchivIndex index;
    QTemporaryFile saveToFile;
    index.saveIndex( &saveToFile );
    QVERIFY( QFile::exists( saveToFile.fileName() ) );
}

void ArchivIndexTest::should_get_file_from_index() {
    ArchivIndex index;
    ArchivFileReference ref1( "filename1", "extractFileName1", 10, 200, 300 );
    ArchivFileReference ref2( "filename2", "extractFileName2", 201, 189, 400 );
    index.addFileReference( &ref1 );
    index.addFileReference( &ref2 );
    ArchivFileReference* ref1ret = index.getFileReferenceByFileName( "filename1" );
    QVERIFY( ref1ret );
    QVERIFY( ref1ret->fileName() == "filename1" );
    ArchivFileReference* refNotInIndex =  index.getFileReferenceByFileName( "asdgasdg" );
    QVERIFY( refNotInIndex == 0 );
}

void ArchivIndexTest::should_add_file_to_index() {
    ArchivIndex index;
    ArchivFileReference ref1( "filename1", "extractFileName1", 10, 200, 300 );
    ArchivFileReference ref2( "filename2", "extractFileName2", 201, 189, 400 );
    index.addFileReference( &ref1 );
    index.addFileReference( &ref2 );
    QList<ArchivFileReference*>* refs = index.fileReferences();
    QVERIFY( refs->count() == 2 );
    QVERIFY( refs->at( 0 )->fileName() == "filename1" );
    QVERIFY( refs->at( 0 )->extractFileName() == "extractFileName1" );
    QVERIFY( refs->at( 0 )->offset() ==  10 );
    QVERIFY( refs->at( 0 )->fileSize() == 200 );
    QVERIFY( refs->at( 0 )->extractedFileSize() == 300 );
    QVERIFY( refs->at( 1 )->fileName() == "filename2" );
    QVERIFY( refs->at( 1 )->extractFileName() == "extractFileName2" );
    QVERIFY( refs->at( 1 )->offset() ==  201 );
    QVERIFY( refs->at( 1 )->fileSize() == 189 );
    QVERIFY( refs->at( 1 )->extractedFileSize() == 400 );
    QVERIFY( index.fileCount() == 2 );
}

void ArchivIndexTest::should_get_file_count() {
    ArchivIndex index;
    ArchivFileReference ref1( "filename1", "extractFileName1", 0, 200, 300 );
    ArchivFileReference ref2( "filename2", "extractFileName2", 200, 150, 400 );
    ArchivFileReference ref3( "filename3", "extractFileName3", 350, 100, 500 );
    index.addFileReference( &ref1 );
    index.addFileReference( &ref2 );
    index.addFileReference( &ref3 );
    QVERIFY2( index.fileCount() == 3 , "Wrong filecont returned" );
}

void ArchivIndexTest::should_calculate_extracedcontentsize() {
    ArchivIndex index;
    ArchivFileReference ref1( "filename1", "extractFileName1", 0, 200, 300 );
    ArchivFileReference ref2( "filename2", "extractFileName2", 200, 150, 400 );
    ArchivFileReference ref3( "filename3", "extractFileName3", 350, 100, 500 );
    index.addFileReference( &ref1 );
    index.addFileReference( &ref2 );
    index.addFileReference( &ref3 );
    QVERIFY2( index.extracedContentSize() == 1200, "extracted Content size not correctly calculated" );
}

void ArchivIndexTest::should_remove_file_from_index() {
    ArchivIndex index;
    ArchivFileReference ref1( "filename1", "extractFileName1", 0, 200, 300 );
    ArchivFileReference ref2( "filename2", "extractFileName2", 200, 150, 400 );
    ArchivFileReference ref3( "filename3", "extractFileName3", 350, 100, 500 );
    index.addFileReference( &ref1 );
    index.addFileReference( &ref2 );
    index.addFileReference( &ref3 );
    QList<ArchivFileReference*>* refs = index.fileReferences();
    // Verify added
    QVERIFY( refs->count() == 3 );
    QVERIFY( refs->at( 0 )->fileName() == "filename1" );
    QVERIFY( refs->at( 0 )->extractFileName() == "extractFileName1" );
    QVERIFY( refs->at( 0 )->offset() ==  0 );
    QVERIFY( refs->at( 0 )->fileSize() == 200 );
    QVERIFY( refs->at( 0 )->extractedFileSize() == 300 );
    QVERIFY( refs->at( 1 )->fileName() == "filename2" );
    QVERIFY( refs->at( 1 )->extractFileName() == "extractFileName2" );
    QVERIFY( refs->at( 1 )->offset() ==  200 );
    QVERIFY( refs->at( 1 )->fileSize() == 150 );
    QVERIFY( refs->at( 1 )->extractedFileSize() == 400 );
    QVERIFY( refs->at( 2 )->fileName() == "filename3" );
    QVERIFY( refs->at( 2 )->extractFileName() == "extractFileName3" );
    QVERIFY( refs->at( 2 )->offset() ==  350 );
    QVERIFY( refs->at( 2 )->fileSize() == 100 );
    QVERIFY( refs->at( 2 )->extractedFileSize() == 500 );
    QVERIFY( index.fileCount() == 3 );
    index.removeFileReference( "filename1" );
    // Verify remove
    refs = index.fileReferences();
    QVERIFY( refs->count() == 2 );
    QVERIFY( refs->at( 0 )->fileName() == "filename2" );
    QVERIFY( refs->at( 0 )->extractFileName() == "extractFileName2" );
    QVERIFY( refs->at( 0 )->offset() ==  0 );
    QVERIFY( refs->at( 0 )->fileSize() == 150 );
    QVERIFY( refs->at( 0 )->extractedFileSize() == 400 );
    QVERIFY( refs->at( 1 )->fileName() == "filename3" );
    QVERIFY( refs->at( 1 )->extractFileName() == "extractFileName3" );
    QVERIFY( refs->at( 1 )->offset() ==  150 );
    QVERIFY( refs->at( 1 )->fileSize() == 100 );
    QVERIFY( refs->at( 1 )->extractedFileSize() == 500 );
    QVERIFY( index.fileCount() == 2 );
}

