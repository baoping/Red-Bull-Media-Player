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
#include "ArchivFileTest.h"

// QT includes
#include <QFileInfo>
#include <QByteArray>
#include <QDataStream>
#include <QUuid>
#include <QTemporaryFile>


// Project includes
#include "../../../RedBullPlayer/Package/ArchivFile.h"


QByteArray testData = QByteArray( "for each piece of music I am working on in LIVE, so that when LIVE 6"
                                  "is released I can get fully into shaping the music & vision together..."
                                  "The random playback idea is really related to timelapse in a way, as"
                                  "I have shot lots of video with a locked off camera on tripod etc"
                                  "over long durations & am interested in doing mroe with it than just"
                                  "speeding it up or slowing it down...."
                                  "I was also incredibly inspired by a short film i saw at the Film festival"
                                  "here, which involved very very clever manipulation of real footage;"
                                  "see this film if you can, its creator would be an interesting person to" );

void ArchivFileTest::should_load_from_qfileinfo() {
    QTemporaryFile testFile;
    QVERIFY( testFile.open() );
    QDataStream out( &testFile );
    out.writeRawData( testData, testData.size() );
    testFile.flush();
    QVERIFY(  testFile.exists() );
    QFileInfo testFileInfo( testFile.fileName() );
    ArchivFile f( testFileInfo, "ExtractFiuleName" );
    QVERIFY( f.fileInfo() ==  testFileInfo );
    QVERIFY( f.fileName() ==  testFileInfo.fileName() );
    QVERIFY( f.extractFileName() == "ExtractFiuleName" );
    QVERIFY( f.fileSize() == testFileInfo.size() );
    QVERIFY( f.compressedDataSize() == qCompress( testData, ArchivFile::COMPRESSION_RATE ).size() );
    QVERIFY( f.data() == testData );
    QVERIFY( f.compressedData() == qCompress( testData, ArchivFile::COMPRESSION_RATE ) );
    ArchivFile f2( testFileInfo );
    QVERIFY( f2.extractFileName() == testFileInfo.fileName() );
}

void ArchivFileTest::should_load_from_qfileinfo_with_newInternalFileName() {
    QTemporaryFile testFile;
    QVERIFY( testFile.open() );
    QDataStream out( &testFile );
    out.writeRawData( testData, testData.size() );
    testFile.flush();
    QVERIFY(  testFile.exists() );
    QFileInfo testFileInfo( testFile.fileName() );
    ArchivFile f( testFileInfo, "ExtractFiuleName", "NewInternalName" );
    QVERIFY( f.fileInfo() ==  testFileInfo );
    QVERIFY( f.fileName() ==  "NewInternalName" );
    QVERIFY( f.extractFileName() == "ExtractFiuleName" );
    QVERIFY( f.fileSize() == testFileInfo.size() );
    QVERIFY( f.compressedDataSize() == qCompress( testData, ArchivFile::COMPRESSION_RATE ).size() );
    QVERIFY( f.data() == testData );
    QVERIFY( f.compressedData() == qCompress( testData, ArchivFile::COMPRESSION_RATE ) );
    ArchivFile f2( testFileInfo );
    QVERIFY( f2.fileName() == testFileInfo.fileName() );
    QVERIFY( f2.extractFileName() == testFileInfo.fileName() );
}


