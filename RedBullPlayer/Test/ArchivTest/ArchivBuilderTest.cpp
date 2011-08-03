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
#include "ArchivBuilderTest.h"

#include <QFile>
#include <QByteArray>

// Project includes
#include "../../../RedBullPlayer/Package/ArchivFile.h"
#include "../../../RedBullPlayer/Package/ArchivFileReference.h"
#include "../../../RedBullPlayer/Package/ArchivData.h"
#include "../../../RedBullPlayer/Package/ArchivDataReader.h"
#include "../../../RedBullPlayer/Package/ArchivBuilder.h"

void ArchivBuilderTest::should_build_archive() {
    new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    QVERIFY( QFile::exists( "TestArchivBuild.exe" ) );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    reader.findSectionOffsets();
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_build_archive_without_modification() {
    new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    QVERIFY( QFile::exists( "TestArchivBuild.exe" ) );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    reader.findSectionOffsets();
    ArchivBuilder modBuilder( QFileInfo( "TestArchivBuild.exe" ) );
    modBuilder.build( *stubInfo, "TestArchivBuild2.exe" );
    QVERIFY( QFile::exists( "TestArchivBuild2.exe" ) );
    QVERIFY( QFileInfo( "TestArchivBuild.exe" ).size() == QFileInfo( "TestArchivBuild2.exe" ).size() );
    QFile::remove( "TestArchivBuild.exe" );
    QFile::remove( "TestArchivBuild2.exe" );
}


void ArchivBuilderTest::should_build_archive_using_stubData() {
    new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    QByteArray stubData;
    QFile stubFile( stubInfo->absoluteFilePath() );
    stubFile.open( QIODevice::ReadOnly );
    stubData = stubFile.readAll();
    stubFile.close();
    ArchivBuilder builder;
    builder.build( stubData, "TestArchivBuild.exe" );
    QVERIFY( QFile::exists( "TestArchivBuild.exe" ) );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    reader.findSectionOffsets();
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_add_file() {
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.addFile( "data1.dat", "extr/data1.dat", QString::null );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    const ArchivIndex& index = builder.index();
    QVERIFY( index.getFileReferenceByFileName( "data1.dat" ) != 0 );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    QByteArray filedata = reader.data();
    QVERIFY( filedata == ArchivFile( *data1info, "exasdf" ).compressedData() );
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_add_file_with_customFileName() {
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.addFile( "data1.dat", "extr/data1.dat", "customFileName" );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    const ArchivIndex& index = builder.index();
    QVERIFY( index.getFileReferenceByFileName( "customFileName" ) != 0 );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    QByteArray filedata = reader.data();
    QVERIFY( filedata == ArchivFile( *data1info, "exasdf", "asdf" ).compressedData() );
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_remove_file() {
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    QFileInfo* data2info = new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.addFile( "data1.dat", "extr/data1.dat", QString::null );
    builder.addFile( "data2.dat", "extr/data2.dat", QString::null );
    const ArchivIndex& index = builder.index();
    QVERIFY( index.getFileReferenceByFileName( "data1.dat" ) != 0 );
    QVERIFY( index.getFileReferenceByFileName( "data2.dat" ) != 0 );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    ArchivDataReader reader( QFileInfo( "TestArchivBuild.exe" ) );
    QByteArray filedata = reader.data();
    QByteArray expectedData;
    expectedData.append( ArchivFile( *data1info, "exasdf" ).compressedData() );
    expectedData.append( ArchivFile( *data2info, "exasdf" ).compressedData() );
    QVERIFY( filedata == expectedData );
    QFile::remove( "TestArchivBuild.exe" );
    builder.removeFile( "data1.dat" );
    const ArchivIndex& index2 = builder.index();
    QVERIFY( index2.getFileReferenceByFileName( "data1.dat" ) == 0 );
    QVERIFY( index2.getFileReferenceByFileName( "data2.dat" ) != 0 );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    ArchivDataReader reader2( QFileInfo( "TestArchivBuild.exe" ) );
    QByteArray filedata2 = reader2.data();
    QByteArray expectedData2;
    expectedData2.append( ArchivFile( *data2info, "exasdf" ).compressedData() );
    QVERIFY( filedata2 == expectedData2 );
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_extract_file() {
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    new QFileInfo( "data2.dat" );
    new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.addFile( "data1.dat", "extr/data1.dat", QString::null );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    ArchivBuilder editModeBuilder( QFileInfo( "TestArchivBuild.exe" ) );
    QByteArray fileData = editModeBuilder.extractFileData( *editModeBuilder.index().getFileReferenceByFileName( "data1.dat" ) );
    QVERIFY( fileData.size() == data1info->size() );
    QFile::remove( "TestArchivBuild.exe" );
}

void ArchivBuilderTest::should_return_extraced_content_size() {
    QFileInfo* data1info = new QFileInfo( "data1.dat" );
    QFileInfo* data2info = new QFileInfo( "data2.dat" );
    QFileInfo* data3info = new QFileInfo( "data3.dat" );
    QFileInfo* stubInfo = new QFileInfo( "data5.dat" );
    ArchivBuilder builder;
    builder.addFile( "data1.dat", "extr/data1.dat", QString::null );
    builder.addFile( "data2.dat", "extr/data2.dat", QString::null );
    builder.addFile( "data3.dat", "extr/data3.dat", QString::null );
    builder.build( *stubInfo, "TestArchivBuild.exe" );
    ArchivBuilder editModeBuilder( QFileInfo( "TestArchivBuild.exe" ) );
    int expectedExtracedSize = data1info->size() + data2info->size() + data3info->size();
    QVERIFY2( editModeBuilder.index().extracedContentSize() ==  expectedExtracedSize, "Exctraced COntent Size not correctly read from archiv" );
    QFile::remove( "TestArchivBuild.exe" );
}
