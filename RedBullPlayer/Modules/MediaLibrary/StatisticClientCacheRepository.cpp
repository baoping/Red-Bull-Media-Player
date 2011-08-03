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
#include "StatisticClientCacheRepository.h"

#include <QSqlQuery>
#include <QMutexLocker>

// Project includes
#include "../../Container/IError.h"
#include "../../ToolLib/UuidCreator.h"

#include "MediaDB_sql.h"
#include "StatisticEntry.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;

StatisticClientCacheRepository::~StatisticClientCacheRepository() {}

StatisticClientCacheRepository::StatisticClientCacheRepository( MediaDB* mediaDb, QObject *parent ) : QObject( parent ) {
    Q_ASSERT( mediaDb != 0 );
    _mediaDb = mediaDb;
}

IStatisticEntry* StatisticClientCacheRepository::createInMemoryEntry( QString entryType, QString data ) {
    Q_ASSERT( ! entryType.isNull() && ! entryType.isEmpty() );
    QUuid id = UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    return new StatisticEntry( id, entryType, data, date );
}

IStatisticEntry* StatisticClientCacheRepository::createEntry( QString entryType, QString data ) {
    Q_ASSERT( ! entryType.isNull() && ! entryType.isEmpty() );
    QMutexLocker locker( &_createEntryMutex );
    QUuid id = UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare( INSERT_STATISTC_CLIENT_CACHE );
    query.bindValue( ":GUID", id.toString() );
    query.bindValue( ":EntryType", entryType );
    query.bindValue( ":Data", data );
    query.bindValue( ":Date", date.toTime_t() );

    if ( ! query.exec() ) {
        return 0;
    }

    return new StatisticEntry( id, entryType, data, date );
}

bool StatisticClientCacheRepository::deleteEntry( QUuid guid, RedBullPlayer::IError* error ) {
    Q_ASSERT( ! guid.isNull() );
    Q_ASSERT( error != 0 );
    QMutexLocker locker( &_deleteEntryMutex );
    QSqlQuery query;
    query.prepare( DELETE_CLIENT_STATISTIC_CACHE );
    query.bindValue( ":GUID", guid.toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( tr( "Could not delete StatistcClientEntry with guid: %1" ).arg( guid.toString() ) );
        return false;
    }

    return true;
}

QList<IStatisticEntry*>* StatisticClientCacheRepository::getAllEntries() {
    QList<IStatisticEntry*>* ret = new QList<IStatisticEntry*>();
    QSqlTableModel model;
    model.setTable( STATISTIC_CLIENT_CACHE_TABLE_NAME );
    model.select();

    for( int i = 0; i < model.rowCount(); i++ ) {
        QUuid guid = model.record( i ).value( "GUID" ).toString();
        QString entryType =  model.record( i ).value( "EntryType" ).toString();
        QString data =  model.record( i ).value( "Data" ).toString();
        uint date = model.record( i ).value( "Date" ).toUInt();
        ret->append( new StatisticEntry( guid, entryType, data, QDateTime::fromTime_t( date ) ) );
    }

    return ret;
}
