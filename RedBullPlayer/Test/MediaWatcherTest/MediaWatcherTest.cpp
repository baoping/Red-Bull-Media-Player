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
#include "MediaWatcherTest.h"

#include <QFile>
#include <QSignalSpy>
#include <QList>
#include <QVariant>
#include <QDebug>
#include <QEventLoop>

#include "../../ToolLib/helper.h"
#include "MediaWatcher.h"

using namespace RedBullPlayer::Modules::MediaWatcher;

MediaWatcherTest::MediaWatcherTest() {
}

void MediaWatcherTest::initTestCase() {
    Helper::mkDirIfNotExists( "testMediaFolder" );
}

void MediaWatcherTest::cleanupTestCase() {
    QDir dir( "testMediaFolder" );
    QFileInfo fileInfo( dir, "testFile.txt" );

    if ( fileInfo.exists() )
        QFile::remove( fileInfo.absoluteFilePath() );

    Helper::rmDirIfExists( "testMediaFolder" );
}

void MediaWatcherTest::should_watch_path_and_emit_file_added() {
    QDir dir( "testMediaFolder" );
    QVERIFY( dir.exists() );
    MediaWatcher* watcher = new MediaWatcher( dir.absolutePath(), this );
    QSignalSpy spyFileAdded( watcher, SIGNAL( fileAdded( QString ) ) );
    watcher->startWatching();
    QVERIFY( watcher->isWatching() );
    QVERIFY( watcher->files().count() == 0 );
    QEventLoop eventLoop;
    connect( watcher, SIGNAL( fileAdded( QString ) ), &eventLoop, SLOT( quit() ) );
    //create test file
    QFileInfo fileInfo( dir, "testFile.txt" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadWrite ) );
    file.write( "TestData" );
    file.flush();
    file.close();
    eventLoop.exec();
    QVERIFY( ( spyFileAdded.count() == 1 ) );
    QVERIFY( watcher->files().count() == 1 );
    QList<QVariant> arguments = spyFileAdded.takeFirst();
    QVERIFY( arguments.at( 0 ).type() == QVariant::String );
    QString argumentFileName = arguments.at( 0 ).toString();
    QVERIFY( argumentFileName == fileInfo.absoluteFilePath() );
    watcher->stopWatching();
    QVERIFY( !watcher->isWatching() );
    delete watcher;
    QFile::remove( fileInfo.absoluteFilePath() );
}


void MediaWatcherTest::should_watch_path_and_emit_file_removed() {
    QDir dir( "testMediaFolder" );
    QVERIFY( dir.exists() );
    //create test file
    QFileInfo fileInfo( dir, "testFile.txt" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadWrite ) );
    file.write( "TestData" );
    file.flush();
    file.close();
    MediaWatcher* watcher = new MediaWatcher( dir.absolutePath(), this );
    QSignalSpy spyFileRemoved( watcher, SIGNAL( fileRemoved( QString ) ) );
    watcher->startWatching();
    QVERIFY( watcher->isWatching() );
    QVERIFY( watcher->files().count() == 1 );
    QEventLoop eventLoop;
    connect( watcher, SIGNAL( fileRemoved( QString ) ), &eventLoop, SLOT( quit() ) );
    QFile::remove( fileInfo.absoluteFilePath() );
    eventLoop.exec();
    QVERIFY( ( spyFileRemoved.count() == 1 ) );
    QVERIFY( watcher->files().count() == 0 );
    QList<QVariant> arguments = spyFileRemoved.takeFirst();
    QVERIFY( arguments.at( 0 ).type() == QVariant::String );
    QString argumentFileName = arguments.at( 0 ).toString();
    QVERIFY( argumentFileName == fileInfo.absoluteFilePath() );
    watcher->stopWatching();
    QVERIFY( !watcher->isWatching() );
    delete watcher;
}
