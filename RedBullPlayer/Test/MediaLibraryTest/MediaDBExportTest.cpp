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
#include "MediaDBExportTest.h"

#include <QFile>
#include <QImage>

#include "MediaDBExport.h"
#include "MediaDB.h"
#include "MediaDB_sql.h"
#include "MediaLibrary/IMediaDB.h"
#include "MediaLibrary/IMediaRepository.h"
#include "MediaLibrary/IMedia.h"
#include "MediaRepository.h"
#include "Media.h"
#include "Error.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

MediaDBExportTest::MediaDBExportTest() {
}

void MediaDBExportTest::should_export_tables() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    MediaDBExport::exportToXml( "testExport.xml" );
    QVERIFY( QFile::exists( "testExport.xml" ) );
}

void MediaDBExportTest::should_export_media_with_cover_image() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    IMedia* media = db->mediaRepository()->createNewMedia( RedBullPlayer::Tools::UuidCreator::create(), "test.txt", *db->audioType() );
    QVERIFY( media != 0 );
    QImage image( "RedBullLogo.png" );
    media->setCoverImage( image );
    Error err;
    QVERIFY( db->mediaRepository()->saveMedia( *media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    MediaDBExport::exportToXml( "testExport.xml" );
    QVERIFY( QFile::exists( "testExport.xml" ) );
}

