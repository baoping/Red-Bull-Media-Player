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
#include "SnapshotRepository.h"

// Qt includes
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QMutexLocker>

// Project includes
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

#include "MediaDB_sql.h"
#include "Snapshot.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

SnapshotRepository::SnapshotRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( mediaDb != 0 );
    _mediaDb = mediaDb;
}

ISnapshot* SnapshotRepository::createSnapshot( QUuid guid, QImage image, float position, IMedia* media ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_createSnapshotMutex );
    return new Snapshot( guid, image, position, media );
}

bool SnapshotRepository::saveSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_saveSnapshotMutex );
    error->reset();
    ISnapshot* s = getSnapshotByGuid( snapshot.guid(), error );

    if ( error->errorType() != Error::NoError ) {
        return false;
    }

    QByteArray ba;
    QBuffer buf( &ba );
    snapshot.image().save( &buf, "PNG" );

    if ( s ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Run update..." );
        QSqlQuery query;
        query.prepare( UPDATE_SNAPSHOT_TEMPLATE );
        query.bindValue( ":GUID", snapshot.guid().toString() );
        query.bindValue( ":Media_GUID", snapshot.media()->guid().toString() );
        query.bindValue( ":Snapshot", ba );
        query.bindValue( ":Position", snapshot.position() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SnapshotRepository::saveSnapshot failed! update sql: " + query.executedQuery() );
            error->setErrorMessage( "Could not save Snapshot", Error::Normal );
            return false;
        }

        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Update done SQL: "  + query.executedQuery() );
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Run insert..." );
        QSqlQuery query;
        query.prepare( INSERT_SNAPSHOT_TEMPLATE );
        query.bindValue( ":GUID", snapshot.guid().toString() );
        query.bindValue( ":Media_GUID", snapshot.media()->guid().toString() );
        query.bindValue( ":Snapshot", ba );
        query.bindValue( ":Position", snapshot.position() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SnapshotRepository::saveSnapshot failed! Insert sql: " + query.executedQuery() );
            error->setErrorMessage( tr( "Could not save Snapshot" ), Error::Normal );
            return false;
        }
    }

    return true;
}
bool SnapshotRepository::deleteSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_deleteSnapshotMutex );
    error->reset();
    QSqlQuery query;
    query.prepare( DELETE_SNAPSHOT_TEMPLATE );
    query.bindValue( ":GUID", snapshot.guid().toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SnapshotRepository::deleteSnapshot failed! delete sql: " + query.executedQuery() );
        error->setErrorMessage(  tr( "Could not delete Snapshot" ), Error::Normal );
        return false;
    }

    return true;
}

ISnapshot* SnapshotRepository::getSnapshotByGuid( QUuid guid, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getSnapshotByGuidMutex );
    QSqlTableModel model;
    model.setTable( SNAPSHOT_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( guid.toString() ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createSnapshotFromSQLresult( model.record( 0 ), 0, error );
    }

    return 0;
}

QList<ISnapshot*>* SnapshotRepository::getSnapshotsByMedia( IMedia* media, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getSnapshotsByMediaMutex );
    QList<ISnapshot*>* ret = new QList<ISnapshot*>();
    QSqlTableModel model;
    model.setTable( SNAPSHOT_TABLE_NAME );
    model.setFilter( QString( "Media_GUID='%1'" ).arg( media->guid().toString() ) );
    model.select();

    for ( int i = 0; i < model.rowCount(); i++ ) {
        ISnapshot* s = createSnapshotFromSQLresult( model.record( i ), media, error );
        ret->append( s );
    }

    return ret;
}

ISnapshot* SnapshotRepository::getSnapshotsByMediaAnPosition( IMedia* media, float position, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getSnapshotsByMediaAnPositionMutex );
    QList<ISnapshot*>* all = getSnapshotsByMedia( media, error );
    ISnapshot* ret = 0;

    for( int i = 0; i < all->count(); i++ ) {
        if ( all->at( i )->position() == position ) {
            ret = all->at( i );
        } else {
            delete all->at( i );
        }
    }

    return ret;
}

ISnapshot* SnapshotRepository::createSnapshotFromSQLresult( QSqlRecord record, IMedia* media, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_createSnapshotFromSQLresultMutex );
    QUuid guid = record.value( "GUID" ).toString();
    QUuid mediaGuid = record.value( "Media_GUID" ).toString();
    QByteArray imageData = record.value( "Snapshot" ).toByteArray();
    QString strPosition = record.value( "Position" ).toString();
    bool ok;
    float position = strPosition.toFloat( &ok );
    Q_ASSERT( ok );

    if ( ! media ) {
        media = _mediaDb->mediaRepository()->getMediaByGuid( mediaGuid );
    }

    if ( ! media ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY,
                                         QString( "Snapshot with id %1 stores reference to non-existing media with id %2" )
                                         .arg( guid ).arg( mediaGuid ) );
        error->setErrorMessage( tr( "Invalid data! "
                                    "Snapshot with id %1 stores reference to non-existing media with id %2" )
                                .arg( guid ).arg( mediaGuid ) );
        return 0;
    }

    QImage img;
    img.loadFromData( imageData );

    if ( img.isNull() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Reading Image from db failed!" );
        error->setErrorMessage( tr( "Invalid data! Reading Image from db faild for snapshot with id: %1" ).arg( guid ) );
        return 0;
    }

    return new Snapshot( guid, img, position, media );
}
