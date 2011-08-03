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
#include "ArchivDataTest.h"

#include <QFile>
#include <QByteArray>

// Project includes
#include "../../../RedBullPlayer/Package/ArchivFile.h"
#include "../../../RedBullPlayer/Package/ArchivFileReference.h"
#include "../../../RedBullPlayer/Package/ArchivData.h"

void ArchivDataTest::should_append_file() {
    ArchivData archiv;
    QFileInfo data1info( "data1.dat" );
    QFileInfo data2info( "data2.dat" );
    QFileInfo data3info( "data3.dat" );
    ArchivFile data1ArchFile( data1info, "extract/data1.dat" );
    ArchivFile data2ArchFile( data2info, "extract/data2.dat" );
    ArchivFile data3ArchFile( data3info, "extract/data3.dat" );
    ArchivFileReference* data1ArchFileRef = archiv.appendFile( &data1ArchFile );
    ArchivFileReference* data2ArchFileRef = archiv.appendFile( &data2ArchFile );
    ArchivFileReference* data3ArchFileRef = archiv.appendFile( &data3ArchFile );
    QByteArray archivData = archiv.data();
    //   qDebug() << "Achive Size: " << archivData.size();
    int expectedSize = data1ArchFile.compressedData().size() + data2ArchFile.compressedData().size() + data3ArchFile.compressedData().size();
    //   qDebug() << "Expected Size: " << expectedSize;
    QVERIFY( archivData.size() == expectedSize );
    QVERIFY( data1ArchFileRef->fileName() == "data1.dat" );
    QVERIFY( data1ArchFileRef->extractFileName() == "extract/data1.dat" );
    QVERIFY( data1ArchFileRef->offset() == 0 );
    QVERIFY( data1ArchFileRef->fileSize() == data1ArchFile.compressedData().size() );
    QVERIFY( data2ArchFileRef->fileName() == "data2.dat" );
    QVERIFY( data2ArchFileRef->extractFileName() == "extract/data2.dat" );
    QVERIFY( data2ArchFileRef->offset() == data1ArchFileRef->fileSize() );
    QVERIFY( data2ArchFileRef->fileSize() == data2ArchFile.compressedData().size() );
    QVERIFY( data3ArchFileRef->fileName() == "data3.dat" );
    QVERIFY( data3ArchFileRef->extractFileName() == "extract/data3.dat" );
    QVERIFY( data3ArchFileRef->offset() == data1ArchFileRef->fileSize() + data2ArchFileRef->fileSize() );
    QVERIFY( data3ArchFileRef->fileSize() == data3ArchFile.compressedData().size() );
    QByteArray allData;
    allData.append( data1ArchFile.compressedData() );
    allData.append( data2ArchFile.compressedData() );
    allData.append( data3ArchFile.compressedData() );
    QVERIFY( allData == archivData );
}

void ArchivDataTest::should_remove_file() {
    ArchivData archiv;
    QFileInfo data1info( "data1.dat" );
    QFileInfo data2info( "data2.dat" );
    QFileInfo data3info( "data3.dat" );
    ArchivFile data1ArchFile( data1info, "extract/data1.dat" );
    ArchivFile data2ArchFile( data2info, "extract/data2.dat" );
    ArchivFile data3ArchFile( data3info, "extract/data3.dat" );
    archiv.appendFile( &data1ArchFile );
    ArchivFileReference* data2ArchFileRef = archiv.appendFile( &data2ArchFile );
    archiv.appendFile( &data3ArchFile );
    archiv.removeFile( *data2ArchFileRef );
    QByteArray archivData = archiv.data();
    // qDebug() << "Achive Size: " << archivData.size();
    int expectedSize = data1ArchFile.compressedData().size() + data3ArchFile.compressedData().size();
    // qDebug() << "Expected Size: " << expectedSize;
    QVERIFY( archivData.size() == expectedSize );
    QByteArray allData;
    allData.append( data1ArchFile.compressedData() );
    allData.append( data3ArchFile.compressedData() );
    QVERIFY( allData == archivData );
}

void ArchivDataTest::should_set_data_test() {
    ArchivData archiv1;
    ArchivData archiv2;
    QFileInfo data1info( "data1.dat" );
    QFileInfo data2info( "data2.dat" );
    QFileInfo data3info( "data3.dat" );
    QFileInfo data4info( "data4.dat" );
    ArchivFile data1ArchFile( data1info, "extract/data1.dat" );
    ArchivFile data2ArchFile( data2info, "extract/data2.dat" );
    ArchivFile data3ArchFile( data3info, "extract/data3.dat" );
    ArchivFile data4ArchFile( data4info, "extract/data4.dat" );
    archiv1.appendFile( &data1ArchFile );
    archiv1.appendFile( &data2ArchFile );
    QByteArray arch1dataShouldBe;
    arch1dataShouldBe.append( data1ArchFile.compressedData() );
    arch1dataShouldBe.append( data2ArchFile.compressedData() );
    QVERIFY( archiv1.data() == arch1dataShouldBe );
    archiv2.appendFile( &data3ArchFile );
    archiv2.appendFile( &data4ArchFile );
    QByteArray arch2dataShouldBe;
    arch2dataShouldBe.append( data3ArchFile.compressedData() );
    arch2dataShouldBe.append( data4ArchFile.compressedData() );
    QVERIFY( archiv2.data() == arch2dataShouldBe );
    archiv2.setData( archiv1.data() );
    QVERIFY( archiv2.data() == arch1dataShouldBe );
}
