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
#include "StatisticClientCacheRepositporyTest.h"

#include <QString>
#include <QDebug>
#include <QUuid>
#include <QImage>
#include <QSqlTableModel>

#include "MediaLibrary/IMediaDB.h"
#include "MediaLibrary/IStatisticClientCacheRepository.h"
#include "MediaLibrary/IStatisticEntry.h"
#include "Shell.h"
#include "Error.h"
#include "SystemMetaDataRepository.h"
#include "MediaDB.h"
#include "MediaDB_sql.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;




void StatisticClientCacheRepositporyTest::should_save_new_entry() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IStatisticClientCacheRepository* rep  = db->statisticClientCacheRepository();
    IStatisticEntry* entry = rep->createEntry( "TestType", "TestData" );
    QVERIFY2( entry != 0, "Entry must not be 0" );
    QVERIFY2( ! entry->guid().isNull(), "Entry must have anon null guid" );
    QVERIFY2( entry->entryType() == "TestType", "Entry type must be: TestType" );
    QVERIFY2( entry->data() == "TestData", "Entry data must be: TestData" );
    QVERIFY2( entry->date() <= QDateTime::currentDateTime(), "Entry Datetimke must be <= curent date" );
    QVERIFY2( entry->date() > QDateTime::currentDateTime().addSecs( -60 ), "Entry Datetimke must be > curent date -60s" );
    QSqlTableModel model;
    model.setTable( STATISTIC_CLIENT_CACHE_TABLE_NAME );
    model.select();
    QVERIFY2(  model.rowCount() > 0, "Data must be stored in DB" );
    bool foundUuid = false;

    for( int i = 0; i < model.rowCount() && ! foundUuid; i++ ) {
        QUuid guid = model.record( i ).value( "GUID" ).toString();

        if ( guid == entry->guid() ) {
            foundUuid = true;
        }
    }

    QVERIFY2( foundUuid, "Entry must be stored in db" );
}

void StatisticClientCacheRepositporyTest::should_delete_entry() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IStatisticClientCacheRepository* rep  = db->statisticClientCacheRepository();
    IStatisticEntry* entry = rep->createEntry( "TestType", "TestData" );
    rep->createEntry( "TestType", "TestData" );
    rep->createEntry( "TestType", "TestData" );
    QVERIFY2( rep->deleteEntry( entry->guid(), &err ), "Must successfully delete enbtry" );
    QVERIFY2( err.errorType() == Error::NoError, "Must not set an error" );
    QSqlTableModel model;
    model.setTable( STATISTIC_CLIENT_CACHE_TABLE_NAME );
    model.select();
    bool foundUuid = false;
    QVERIFY2( model.rowCount() == 2, "Must return currect entry count" );

    for( int i = 0; i < model.rowCount() && ! foundUuid; i++ ) {
        QUuid guid = model.record( i ).value( "GUID" ).toString();

        if ( guid == entry->guid() ) {
            foundUuid = true;
        }
    }

    QVERIFY2( ! foundUuid, "Entry must not be be stored in db anymore" );
}

void StatisticClientCacheRepositporyTest::should_getall_entries() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IStatisticClientCacheRepository* rep  = db->statisticClientCacheRepository();
    IStatisticEntry* e1 = rep->createEntry( "TestType", "TestData" );
    IStatisticEntry* e2 = rep->createEntry( "TestType", "TestData" );
    IStatisticEntry* e3 = rep->createEntry( "TestType", "TestData" );
    QList<IStatisticEntry*>* entries = rep->getAllEntries();
    QVERIFY2( entries->count() == 3, "Must return correct entry count" );
    bool founde1 = false;
    bool founde2 = false;
    bool founde3 = false;

    for ( int i = 0; i < entries->count(); i++ ) {
        IStatisticEntry* e = entries->at( i );

        if ( e->guid() == e1->guid() ) founde1 = true;

        if ( e->guid() == e2->guid() ) founde2 = true;

        if ( e->guid() == e3->guid() ) founde3 = true;
    }

    QVERIFY2( founde1 && founde2 && founde3, "Must return all inserted entries" );
}

void StatisticClientCacheRepositporyTest::should_create_but_not_save_entry() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IStatisticClientCacheRepository* rep  = db->statisticClientCacheRepository();
    IStatisticEntry* entry = rep->createInMemoryEntry( "TestType", "TestData" );
    QVERIFY2( entry != 0, "Entry must not be 0" );
    QVERIFY2( ! entry->guid().isNull(), "Entry must have anon null guid" );
    QVERIFY2( entry->entryType() == "TestType", "Entry type must be: TestType" );
    QVERIFY2( entry->data() == "TestData", "Entry data must be: TestData" );
    QVERIFY2( entry->date() <= QDateTime::currentDateTime(), "Entry Datetimke must be <= curent date" );
    QVERIFY2( entry->date() > QDateTime::currentDateTime().addSecs( -60 ), "Entry Datetimke must be > curent date -60s" );
    QSqlTableModel model;
    model.setTable( STATISTIC_CLIENT_CACHE_TABLE_NAME );
    model.select();
    QVERIFY2(  model.rowCount() == 0, "Data must be stored in DB" );
    bool foundUuid = false;

    for( int i = 0; i < model.rowCount() && ! foundUuid; i++ ) {
        QUuid guid = model.record( i ).value( "GUID" ).toString();

        if ( guid == entry->guid() ) {
            foundUuid = true;
        }
    }

    QVERIFY2( ! foundUuid, "Entry must be stored in db" );
}

