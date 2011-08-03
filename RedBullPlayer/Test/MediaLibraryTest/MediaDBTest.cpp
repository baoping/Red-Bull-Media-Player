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
#include "MediaDBTest.h"

// Qt includes
#include <QString>
#include <QStringList>
#include <QtSql>
#include <QSqlTableModel>

// Proj includes
#include "MediaDB.h"
#include "MediaDB_sql.h"
#include "MediaLibrary/IMediaDB.h"




using namespace RedBullPlayer::Modules::MediaLibrary;

MediaDBTest::MediaDBTest() {
}

void MediaDBTest::ctor_WithDbName() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    QVERIFY( db->databaseName() == ":memory:" );
    QVERIFY( db->tagRepository() != 0 );
    QVERIFY( db->mediaRepository() != 0 );
    QVERIFY( db->snapshotRepository() != 0 );
    QVERIFY( db->contentRepository() != 0 );
    QVERIFY( db->newsRepository() != 0 );
    QVERIFY( db->systemMetaDataRepository() != 0 );
    QVERIFY( db->statisticClientCacheRepository() != 0 );
}

void MediaDBTest::enSureTables() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    QStringList tables = db->db().tables();
    QVERIFY( tables.contains( MEDIA_TABLE_NAME ) );
    QVERIFY( tables.contains( MEDIA_TYPE_TABLE_NAME ) );
    QVERIFY( tables.contains( TAG_TABLE_NAME ) );
    QVERIFY( tables.contains( MEDIA_TAG_TABLE_NAME ) );
    QVERIFY( tables.contains( SNAPSHOT_TABLE_NAME ) );
    QVERIFY( tables.contains( NEWS_TABLE_NAME ) );
    QVERIFY( tables.contains( NEWS_IMAGE_TABLE_NAME ) );
    QVERIFY( tables.contains( SYSTEMMETADATA_TABLE_NAME ) );
    QVERIFY( tables.contains( STATISTIC_CLIENT_CACHE_TABLE_NAME ) );
}

void MediaDBTest::enSureDefaultData() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    QSqlTableModel model;
    model.setTable( MEDIA_TYPE_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( MEDIA_TYPE_AUDIO ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    model.clear();
    model.setTable( MEDIA_TYPE_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( MEDIA_TYPE_VIDEO ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    model.clear();
    model.setTable( MEDIA_TYPE_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( MEDIA_TYPE_IMAGE ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    model.clear();
    model.setTable( MEDIA_TYPE_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( MEDIA_TYPE_TV ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    model.clear();
    model.setTable( SYSTEMMETADATA_TABLE_NAME );
    model.setFilter( QString( "DataKey='%1'" ).arg( DB_VERSION_METADATA_DATAKEY ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    model.clear();
    model.setTable( SYSTEMMETADATA_TABLE_NAME );
    model.setFilter( QString( "DataKey='%1'" ).arg( PLAYER_UUID_METADATA_DATAKEY ) );
    model.select();
    QVERIFY( model.rowCount() == 1 );
    QVERIFY( db->videoType() != 0 );
    QVERIFY( db->audioType() != 0 );
    QVERIFY( db->imageType() != 0 );
    QVERIFY( db->tvChannelType() != 0 );
    QVERIFY( db->videoType()->guid() == MEDIA_TYPE_VIDEO );
    QVERIFY( db->audioType()->guid() == MEDIA_TYPE_AUDIO );
    QVERIFY( db->imageType()->guid() == MEDIA_TYPE_IMAGE );
    QVERIFY( db->tvChannelType()->guid() == MEDIA_TYPE_TV );
}

void MediaDBTest::mediaTypes_initOnUse() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    QList<IMediaType*> list = db->mediaTypes();
    QVERIFY( list.count() == 5 );
    bool haveAudio = false;
    bool haveVideo = false;
    bool haveImage = false;
    bool haveTV = false;

    for( int i = 0; i < list.count(); i++ ) {
        if ( list.at( i )->guid() == MEDIA_TYPE_AUDIO ) haveAudio = true;

        if ( list.at( i )->guid() == MEDIA_TYPE_VIDEO ) haveVideo = true;

        if ( list.at( i )->guid() == MEDIA_TYPE_IMAGE ) haveImage = true;

        if ( list.at( i )->guid() == MEDIA_TYPE_TV ) haveTV = true;
    }

    QVERIFY( haveAudio );
    QVERIFY( haveVideo );
    QVERIFY( haveImage );
    QVERIFY( haveTV );
}
