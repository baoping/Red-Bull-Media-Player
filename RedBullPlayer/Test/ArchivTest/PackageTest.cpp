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
#include "PackageTest.h"

#include <QFileInfo>
#include <QDir>

#include "../../../RedBullPlayer/Package/Package.h"

void PackageTest::should_setup_path() {
    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    QFileInfo test1( "testDir/test.dll" );
    p->enshurePathForFile( test1 );
    QVERIFY( QDir( "testDir" ).exists() );
    QFileInfo test2( "testDirNested/testDirNested/testDirNested/testDirNested/testDirNested/test.dll" );
    p->enshurePathForFile( test2 );
    QVERIFY( QDir( "testDirNested/testDirNested/testDirNested/testDirNested/testDirNested" ).exists() );
    QDir().rmpath( "testDir" );
    QDir().rmpath( "testDirNested/testDirNested/testDirNested/testDirNested/testDirNested" );
    p->deleteLater();
}

void PackageTest::should_write_file_to() {
    QByteArray data;
    char* dummy = "1";

    for( int i = 0; i < 1024; i++ ) {
        data.append( dummy, 1 );
    }

    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    QFileInfo outFileInf( "testPath/TestPath/testFile.txt" );
    p->writeFileTo( data, outFileInf );
    QVERIFY( QFile::exists( outFileInf.absoluteFilePath() ) );
    QFile f( outFileInf.absoluteFilePath() );
    f.open( QIODevice::ReadOnly );
    QByteArray fileData = f.readAll();
    f.close();
    QVERIFY(  data == fileData );
    f.remove();
    QDir().rmpath( "testPath/TestPath" );
    p->deleteLater();
}

void PackageTest::should_build_package() {
    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    p->addFile( "data2.dat", "extractPath/data1.dat" );
    p->addFile( "data3.dat", "extractPath/data3.dat" );
    p->addFile( "data4.dat", "extractPath/data4.dat" );
    p->save( "testPackage" );
    QVERIFY( QFile::exists( "testPackage" ) );
    p->deleteLater();
    Package* p2 = Package::openEdit( QFileInfo( "testPackage" ) );
    p2->removeFile( "data3.dat" );
    p2->save( "testPackage2" );
    QVERIFY( QFile::exists( "testPackage2" ) );
    QVERIFY( QFileInfo( "testPackage" ).size() >  QFileInfo( "testPackage2" ).size() );
    QFile( "testPackage" ).remove();
    QFile( "testPackage2" ).remove();
    p2->deleteLater();
}

void PackageTest::should_extract_package() {
    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    p->addFile( "data2.dat", "extractPath/data2.dat" );
    p->addFile( "data3.dat", "extractPath/data3.dat" );
    p->addFile( "data4.dat", "extractPath/data4.dat" );
    p->save( "testPackage" );
    p->deleteLater();
    Package* p2 = Package::openEdit( QFileInfo( "testPackage" ) );
    p2->extractAll( "basePath" );
    QVERIFY( QFile::exists( "basePath/extractPath/data2.dat" ) );
    QVERIFY( QFile::exists( "basePath/extractPath/data3.dat" ) );
    QVERIFY( QFile::exists( "basePath/extractPath/data4.dat" ) );
    p2->extractAll();
    QVERIFY( QFile::exists( "extractPath/data2.dat" ) );
    QVERIFY( QFile::exists( "extractPath/data3.dat" ) );
    QVERIFY( QFile::exists( "extractPath/data4.dat" ) );
    p2->deleteLater();
    QFile( "basePath/extractPath/data2.dat" ).remove();
    QFile( "basePath/extractPath/data3.dat" ).remove();
    QFile( "basePath/extractPath/data4.dat" ).remove();
    QFile( "extractPath/data2.dat" ).remove();
    QFile( "extractPath/data3.dat" ).remove();
    QFile( "extractPath/data4.dat" ).remove();
    QFile( "testPackage" ).remove();
}

void PackageTest::should_return_extracted_size() {
    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    p->addFile( "data2.dat", "extractPath/data2.dat" );
    p->addFile( "data3.dat", "extractPath/data3.dat" );
    p->addFile( "data4.dat", "extractPath/data4.dat" );
    int expectedExtracedSize = QFileInfo( "data2.dat" ).size() + QFileInfo( "data3.dat" ).size() + QFileInfo( "data4.dat" ).size();
    p->save( "testPackage" );
    p->deleteLater();
    Package* p2 = Package::openEdit( QFileInfo( "testPackage" ) );
    QVERIFY2( p2->requiredDiskSpace() == expectedExtracedSize, "Required diskspace not correctly calculated" );
    QFile( "testPackage" ).remove();
}

void PackageTest::should_extract_file() {
    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
    p->addFile( "data2.dat", "extractPath/data2.dat" );
    p->addFile( "data3.dat", "extractPath/data3.dat" );
    p->addFile( "data4.dat", "extractPath/data4.dat" );
    p->save( "testPackage" );
    p->deleteLater();
    Package* p2 = Package::openEdit( QFileInfo( "testPackage" ) );
    p2->extractFile( "data4.dat", "extractPath2/singleExtractedData4.dat" );
    QVERIFY( QFile::exists( "extractPath2/singleExtractedData4.dat" ) );
    QVERIFY( QFileInfo( "data4.dat" ).size() ==  QFileInfo( "extractPath2/singleExtractedData4.dat" ).size() );
    QFile( "extractPath2/singleExtractedData4.dat" ).remove();
    QFile( "testPackage" ).remove();
}
