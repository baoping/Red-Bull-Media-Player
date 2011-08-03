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
#include "SystemMetaDataRepository.h"

// Qt includes
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QImage>
#include <QMutexLocker>
#include <QString>

// Project includes
#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"
#include "MediaDB_sql.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

SystemMetaDataRepository::SystemMetaDataRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    _mediaDb = mediaDb;
}

SystemMetaDataRepository::~SystemMetaDataRepository() {}

QString SystemMetaDataRepository::getMetaData( const QString& dataKey ) {
    QMutexLocker locker( &_getMetaDataMutex );
    QSqlQuery query;
    query.prepare( SELECT_SYSTEMMETADATA_BY_DATAKEY );
    query.bindValue( ":DataKey", dataKey );

    if ( ! query.exec() ) {
        qWarning() << "Query: " << query.executedQuery();
        qFatal( "Invalid query" );
    }

    QSqlQueryModel model;
    model.setQuery( query );
    return ( model.rowCount() == 1 ) ? model.record( 0 ).value( "DataValue" ).toString() : QString::null;
}

QString SystemMetaDataRepository::getDBVersion() {
    return getMetaData( DB_VERSION_METADATA_DATAKEY );
}

QUuid SystemMetaDataRepository::getPlayerUUID() {
    QString strUUID = getMetaData( PLAYER_UUID_METADATA_DATAKEY );
    return QUuid( strUUID );
}

bool SystemMetaDataRepository::allowCollectStatistic() {
    QString retVal = getMetaData( PLAYER_ALLOW_COLLECT_STATISTIC_DATAKEY );
    return ( retVal == QString::null ) ? false : ( retVal == "true" );
}

void SystemMetaDataRepository::setAllowCollectStatistic( bool allowCollectStatistic ) {
    QSqlQuery query;

    if ( getMetaData( PLAYER_ALLOW_COLLECT_STATISTIC_DATAKEY ) == "" ) {
        query.prepare( INSERT_SYSTEMMETADATA_TEMPLATE );
    } else {
        query.prepare( UPDATE_SYSTEMMETADATA_TEMPLATE );
    }

    query.bindValue( ":DataKey", PLAYER_ALLOW_COLLECT_STATISTIC_DATAKEY );
    query.bindValue( ":DataValue", allowCollectStatistic ? "true" : "false" );

    if ( !query.exec() ) {
        //qError() << "Couldn't save CollectStatistic SystemMetaData.";
    }
}

bool SystemMetaDataRepository::allowNetworkConnection() {
    QString retVal = getMetaData( PLAYER_ALLOW_NETWORK_CONNECTION_DATAKEY );
    return ( retVal == QString::null ) ? false : ( retVal == "true" );
}

void SystemMetaDataRepository::setAllowNetworkConnection( bool allowNetworkConnection ) {
    QSqlQuery query;

    if ( getMetaData( PLAYER_ALLOW_NETWORK_CONNECTION_DATAKEY ) == "" ) {
        query.prepare( INSERT_SYSTEMMETADATA_TEMPLATE );
    } else {
        query.prepare( UPDATE_SYSTEMMETADATA_TEMPLATE );
    }

    query.bindValue( ":DataKey", PLAYER_ALLOW_NETWORK_CONNECTION_DATAKEY );
    query.bindValue( ":DataValue", allowNetworkConnection ? "true" : "false" );

    if ( !query.exec() ) {
        //qError() << "Couldn't save DownloadContent SystemMetaData.";
    }
}

bool SystemMetaDataRepository::getShowUserSettingsDialog() {
    QString retVal = getMetaData( PLAYER_SHOW_USER_SETTINGS_DIALOG_DATAKEY );
    return ( retVal == QString::null ) ? true : ( retVal == "true" );
}

void SystemMetaDataRepository::setShowUserSettingsDialog( bool showUserSettingsDialog ) {
    QSqlQuery query;

    if ( getMetaData( PLAYER_SHOW_USER_SETTINGS_DIALOG_DATAKEY ) == "" ) {
        query.prepare( INSERT_SYSTEMMETADATA_TEMPLATE );
    } else {
        query.prepare( UPDATE_SYSTEMMETADATA_TEMPLATE );
    }

    query.bindValue( ":DataKey", PLAYER_SHOW_USER_SETTINGS_DIALOG_DATAKEY );
    query.bindValue( ":DataValue", showUserSettingsDialog ? "true" : "false" );

    if ( !query.exec() ) {
        //qError() << "Couldn't save ShowUserSettingsDialog SystemMetaData.";
    }
}
