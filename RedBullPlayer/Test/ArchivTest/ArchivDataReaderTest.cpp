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
#include "ArchivDataReaderTest.h"


#include <QFile>
#include <QByteArray>

// Project includes
#include "../../../RedBullPlayer/Package/ArchivFile.h"
#include "../../../RedBullPlayer/Package/ArchivFileReference.h"
#include "../../../RedBullPlayer/Package/ArchivData.h"
#include "../../../RedBullPlayer/Package/ArchivDataReader.h"
#include "../../../RedBullPlayer/Package/ArchivBuilder.h"

void ArchivDataReaderTest::should_read_file() {
    ArchivFile *data1ArchFile, *data2ArchFile, *data3ArchFile;
    ArchivFileReference *data1ArchFileRef, *data2ArchFileRef, *data3ArchFileRef;
    QTemporaryFile* tmpArchiv = buildFakedArchiv( 41234, 12344,
                                &data1ArchFile, &data1ArchFileRef,
                                &data2ArchFile, &data2ArchFileRef,
                                &data3ArchFile, &data3ArchFileRef
                                                );
    qDebug() << "created fake arch of size: " << tmpArchiv->size();
    ArchivDataReader reader( QFileInfo( tmpArchiv->fileName() ) );
    //    qDebug() << data1ArchFileRef->fileSize();
    QByteArray data1compressedData = reader.compressedFileData( *data1ArchFileRef );
    QByteArray data1data = reader.fileData( *data1ArchFileRef );
    QVERIFY( data1compressedData == data1ArchFile->compressedData() );
    QVERIFY( data1data == data1ArchFile->data() );
    QByteArray data2compressedData = reader.compressedFileData( *data2ArchFileRef );
    QByteArray data2data = reader.fileData( *data2ArchFileRef );
    QVERIFY( data2compressedData == data2ArchFile->compressedData() );
    QVERIFY( data2data == data2ArchFile->data() );
    QByteArray data3compressedData = reader.compressedFileData( *data3ArchFileRef );
    QByteArray data3data = reader.fileData( *data3ArchFileRef );
    QVERIFY( data3compressedData == data3ArchFile->compressedData() );
    QVERIFY( data3data == data3ArchFile->data() );
    tmpArchiv->deleteLater();
}

void ArchivDataReaderTest::should_find_section_offsets() {
    ArchivFile *data1ArchFile, *data2ArchFile, *data3ArchFile;
    ArchivFileReference *data1ArchFileRef, *data2ArchFileRef, *data3ArchFileRef;
    QTemporaryFile* tmpArchiv = buildFakedArchiv( 41234, 12344,
                                &data1ArchFile, &data1ArchFileRef,
                                &data2ArchFile, &data2ArchFileRef,
                                &data3ArchFile, &data3ArchFileRef
                                                );
    ArchivDataReader reader( QFileInfo( tmpArchiv->fileName() ) );
    reader.findSectionOffsets();
    QString indexSizeString;
    indexSizeString.setNum( 12344 );
    int expectedIndexOffset = 41234 + ArchivBuilder::DATA_MARKER_LENGTH + indexSizeString.toAscii().size() + 1;
    QVERIFY( reader.indexSectionOffset() == expectedIndexOffset );
    QVERIFY( reader.dataSeciotnOffset() == expectedIndexOffset + 12344 );
    tmpArchiv->deleteLater();
}

void ArchivDataReaderTest::should_return_index_data_test() {
    ArchivFile *data1ArchFile, *data2ArchFile, *data3ArchFile;
    ArchivFileReference *data1ArchFileRef, *data2ArchFileRef, *data3ArchFileRef;
    QTemporaryFile* tmpArchiv = buildFakedArchiv( 41234, 12344,
                                &data1ArchFile, &data1ArchFileRef,
                                &data2ArchFile, &data2ArchFileRef,
                                &data3ArchFile, &data3ArchFileRef
                                                );
    ArchivDataReader reader( QFileInfo( tmpArchiv->fileName() ) );
    QByteArray indexData = reader.compressedIndexData();
    QVERIFY( indexData.size() == 12344 );
    tmpArchiv->deleteLater();
}

void ArchivDataReaderTest::should_return_file_data_test() {
    ArchivFile *data1ArchFile, *data2ArchFile, *data3ArchFile;
    ArchivFileReference *data1ArchFileRef, *data2ArchFileRef, *data3ArchFileRef;
    QTemporaryFile* tmpArchiv = buildFakedArchiv( 41234, 12344,
                                &data1ArchFile, &data1ArchFileRef,
                                &data2ArchFile, &data2ArchFileRef,
                                &data3ArchFile, &data3ArchFileRef
                                                );
    ArchivDataReader reader( QFileInfo( tmpArchiv->fileName() ) );
    QByteArray fileData = reader.data();
    QByteArray fileDataShouldBe;
    fileDataShouldBe.append( data1ArchFile->compressedData() );
    fileDataShouldBe.append( data2ArchFile->compressedData() );
    fileDataShouldBe.append( data3ArchFile->compressedData() );
    QVERIFY( fileData == fileDataShouldBe );
    tmpArchiv->deleteLater();
}

void ArchivDataReaderTest::should_return_exestub_data_test() {
    ArchivFile *data1ArchFile, *data2ArchFile, *data3ArchFile;
    ArchivFileReference *data1ArchFileRef, *data2ArchFileRef, *data3ArchFileRef;
    QTemporaryFile* tmpArchiv = buildFakedArchiv( 41234, 12344,
                                &data1ArchFile, &data1ArchFileRef,
                                &data2ArchFile, &data2ArchFileRef,
                                &data3ArchFile, &data3ArchFileRef
                                                );
    ArchivDataReader reader( QFileInfo( tmpArchiv->fileName() ) );
    QByteArray exeStubData = reader.exeStubData();
    QVERIFY( exeStubData.size() == 41234 );
    QByteArray expectedExeData;
    char* dummyStub = "0";

    for( int i = 0; i < 41234; i++ ) {
        expectedExeData.append( dummyStub, 1 );
    }

    QVERIFY( exeStubData == expectedExeData );
    tmpArchiv->deleteLater();
}


QTemporaryFile* ArchivDataReaderTest::buildFakedArchiv( qint64 stubSize, qint64 indexSize,
        ArchivFile** data1ArchFile,
        ArchivFileReference** data1ArchFileRef,
        ArchivFile** data2ArchFile,
        ArchivFileReference** data2ArchFileRef,
        ArchivFile** data3ArchFile,
        ArchivFileReference** data3ArchFileRef ) {
    QTemporaryFile* tmpArchiv = new QTemporaryFile();
    tmpArchiv->open();
    QDataStream ds( tmpArchiv );
    qDebug() << "Write dummy stub of size: " << stubSize;
    char* dummyStub = "0";

    for( qint64 i = 0; i < stubSize; i++ ) {
        ds.writeRawData( dummyStub, 1 );
    }

    // Set dataMarker
    for( uint i = 0; i < ArchivBuilder::DATA_MARKER_LENGTH - 1; i++ ) {
        ds.writeRawData( ArchivBuilder::DATA_MARKER + i , 1 );
        //Q_ASSERT( status != -1 ); WippelDa
    }

    qDebug() << "Marker size: " << ArchivBuilder::DATA_MARKER_LENGTH;
    ds.writeRawData( ArchivBuilder::DATA_MARKER_REPLACECHAR + 0 , 1 );
    // int indexSize = 783245;
    qDebug() << "Index size: " << indexSize;
    QString indexSizeString;
    indexSizeString.setNum( indexSize );
    ds.writeRawData( indexSizeString.toAscii(), indexSizeString.toAscii().size() );
    ds.writeRawData( "\n", 1 );
    qDebug() << "Wrote size string: " << indexSizeString.toAscii();
    char* dummyIndex = "1";

    for( qint64 i = 0; i < indexSize; i++ ) {
        ds.writeRawData( dummyIndex, 1 );
    }

    ArchivData archiv;
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    QFileInfo* data2info = new QFileInfo( "data2.dat" );
    QFileInfo* data3info = new QFileInfo( "data3.dat" );
    *data1ArchFile = new ArchivFile( *data1info, "extract/data1.dat" );
    *data2ArchFile = new ArchivFile( *data2info, "extract/data2.dat" );
    *data3ArchFile = new ArchivFile( *data3info, "extract/data3.dat" );
    *data1ArchFileRef = archiv.appendFile( *data1ArchFile );
    *data2ArchFileRef = archiv.appendFile( *data2ArchFile );
    *data3ArchFileRef = archiv.appendFile( *data3ArchFile );
    QByteArray archivData = archiv.data();
    ds.writeRawData( archivData, archivData.size() );
    return tmpArchiv;
}
