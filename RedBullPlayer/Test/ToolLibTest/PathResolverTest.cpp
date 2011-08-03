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
#include "PathResolverTest.h"


#include "FileSystem/PathResolver.h"

using namespace RedBullPlayer::Tools;

void PathResolverTest::should_create_resolvable_path_in_baseDir() {
    QString resovalblePath = PathResolver::createResolvablePath(  QDir::toNativeSeparators( "/xyz/abc/theFileName.txt" ),
                             QDir::toNativeSeparators( "/xyz/abc/" ),  "[placeholder]"  );
    QVERIFY2( QDir::toNativeSeparators( resovalblePath ) == "[placeholder]theFileName.txt", "Path must be created correctly" );
}

void PathResolverTest::should_create_resolvable_path_in_subdir_of_baseDir() {
    QString resovalblePath = PathResolver::createResolvablePath(  QDir::toNativeSeparators( "/xyz/abc/otherDir/theFileName.txt" ),
                             QDir::toNativeSeparators( "/xyz/abc/" ),
                             "[placeholder]"  );
    QVERIFY2( QDir::toNativeSeparators( resovalblePath ) ==  QDir::toNativeSeparators( "[placeholder]otherDir/theFileName.txt" ), "Path must be created correctly" );
}


void PathResolverTest::should_only_build_if_baseDir_and_filePAth_areAbsolute() {
    QString resovalblePath = PathResolver::createResolvablePath(
                                 QDir::toNativeSeparators( "/xyz/abc/otherDir/theFileName.txt" ),
                                 QDir::toNativeSeparators( "asdfd/asdf" ),
                                 "[placeholder]"  );
    QVERIFY2( QDir::toNativeSeparators( resovalblePath ) ==  QDir::toNativeSeparators( "/xyz/abc/otherDir/theFileName.txt" ), "Path must not be manipuzlated due to non absolue  base Dir" );
    resovalblePath = PathResolver::createResolvablePath( QDir::toNativeSeparators( "../otherDir/theFileName.txt" ),
                     QDir::toNativeSeparators( "/asdfd/asdf" ),
                     "[placeholder]"  );
    QVERIFY2( QDir::toNativeSeparators( resovalblePath ) == QDir::toNativeSeparators( "../otherDir/theFileName.txt" ), "Path must not be manipuzlated due to non absolute filepath" );
}

void  PathResolverTest::should_only_build_if_filePath_starts_with_baseDir() {
    QString resovalblePath = PathResolver::createResolvablePath(
                                 QDir::toNativeSeparators( "/xyz/abc/otherDir/theFileName.txt" ),
                                 QDir::toNativeSeparators( "/abc/otherDir" ),
                                 "[placeholder]"  );
    QVERIFY2( QDir::toNativeSeparators( resovalblePath ) ==  QDir::toNativeSeparators( "/xyz/abc/otherDir/theFileName.txt" ), "Path must not be manipuzlated file path contains base dir in the middle " );
}

void PathResolverTest::should_only_resolve_if_baseDir_isAbsolute() {
    QString placeholder = "[placeholder]";
    QString baseDir = QDir::toNativeSeparators( "asdfadsf/asdfas" );
    QVERIFY2( PathResolver::resolvePath( "[placeholder]asdfasdf/asdfadsf/",
                                         baseDir,
                                         placeholder )
              == "[placeholder]asdfasdf/asdfadsf/", "Must not manupulte path due to non absolute basedir" );
}

void PathResolverTest::should_only_resolve_if_path_starts_with_placeholder() {
    QString placeholder = "[placelder]";
    QString baseDir = QDir::toNativeSeparators( "asdfadsf/asdfas" );
    QVERIFY2( PathResolver::resolvePath( "[placeholder]asdfasdf/asdfadsf/",
                                         baseDir,
                                         placeholder )
              == "[placeholder]asdfasdf/asdfadsf/", "Must not manupulte path as it does noit start with the placeholder" );
    QVERIFY2( PathResolver::resolvePath( "asdasd/[placelder]asdfasdf/asdfadsf/",
                                         baseDir,
                                         placeholder )
              == "asdasd/[placelder]asdfasdf/asdfadsf/", "Must not manupulte path as it does noit start with the placeholder" );
}

void PathResolverTest::should_resolve_path() {
    QVERIFY2( PathResolver::resolvePath( "[placeholder]myfileDir/myfile.txt",
                                         "/my/baseDir",
                                         "[placeholder]" )
              == "/my/baseDir/myfileDir/myfile.txt", "Must reslove manupulte path basedir without trailing slash" );
    QVERIFY2( PathResolver::resolvePath( "[placeholder]myfileDir/myfile.txt",
                                         "/my/baseDir/",
                                         "[placeholder]" )
              == "/my/baseDir/myfileDir/myfile.txt", "Must reslove manupulte path basedir without trailing slash" );
}
