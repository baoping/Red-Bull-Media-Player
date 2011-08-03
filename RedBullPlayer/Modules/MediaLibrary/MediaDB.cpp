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
#include "MediaDB.h"

// QT includes
#include <QDebug>
#include <QString>
#include <QFile>
#include <QUuid>
#include <QTextStream>
#include <QSqlTableModel>

// Project includes
#include "MediaDB_sql.h"
#include "Content/Content_sql.h"
#include "MediaType.h"
#include "TagRepository.h"
#include "MediaRepository.h"
#include "SnapshotRepository.h"
#include "News/NewsRepository.h"
#include "MediaDBExport.h"
#include "SystemMetaDataRepository.h"
#include "StatisticClientCacheRepository.h"
#include "Content/ContentRepository.h"

//logging
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

// Config Manager
#include "../../ToolLib/ConfigManager.h"

// UUID gen
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;


MediaDB::MediaDB( QObject* parent, const QString& databaseName ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    _mediaTypes = 0;
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create TagRepository" );
    _tagRepository = 0;
    _tagRepository = new TagRepository( this, this );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create MediaRepository" );
    _mediaRepository = 0;
    _mediaRepository = new MediaRepository( this, this );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create SnapshotRepository" );
    _snapshotRepository = 0;
    _snapshotRepository = new SnapshotRepository( this, this );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create NewsRepository" );
    _newsRepository = 0;
    _newsRepository = new NewsRepository( this, this );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create SystemMetaDataRepository" );
    _systemMetaDataRepository = 0;
    _systemMetaDataRepository = new SystemMetaDataRepository( this, this );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create ContentRepository" );
    _contentRepository = 0;
    _contentRepository = new ContentRepository( this, this );
    _statisticClientCacheRepository = 0;
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Create StatisticClientCacheRepository" );
    _statisticClientCacheRepository = new StatisticClientCacheRepository( this, this );
    QString dbName;

    if ( databaseName.isNull() ) {
        _dbName = ConfigManager::instance()->getMediaLibraryFileName();
    } else {
        _dbName = databaseName;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "DatabaseName: " + _dbName );

    // ensureSchema();
    if ( ! QSqlDatabase::isDriverAvailable( "QSQLITE" ) ) {
        SimpleLogger::instance()->fatal( Constants::LOGGER_MEDIALIBRARY, "QSQLite Driver not available!" );
        //qFatal( "QSQLITE Driver not available!" );
    }

    QSqlDatabase conn = QSqlDatabase::addDatabase( "QSQLITE" );
    conn.setDatabaseName( _dbName );
    SimpleLogger::instance()->fatal( Constants::LOGGER_MEDIALIBRARY, "Open DbConnection" );
    Q_ASSERT( conn.open() ) ;
}

MediaDB::~MediaDB() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );

    while ( !_mediaTypes->isEmpty() ) {
        delete _mediaTypes->takeFirst();
    }

    delete _mediaTypes;
    _mediaTypes = 0;
}

void MediaDB::ensureSchema() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QSqlDatabase conn = db();
    QSqlQuery query( conn );

    if ( ! query.exec( SETUP_SYSTEMMETADATA_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (SYSTEMMETADATA) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_SYSTEMMETADATA_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_TAG_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (TAG) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_TAG_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_MEDIATYPE_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (MEDIATYPE) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_MEDIATYPE_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_MEDIA_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (MEDIA) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_MEDIA_TABLE_SQL );
        return;
    }

    if ( ! query.exec( OPTIMIZE_MEDIA_TABLE_SQL_1 ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Optimize Schema (MEDIA) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + OPTIMIZE_MEDIA_TABLE_SQL_1 );
        return;
    }

    if ( ! query.exec( SETUP_MEDIATAG_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (MEDIATAG) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_MEDIATAG_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_SNAPSHOT_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (SNAPSHOT) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_SNAPSHOT_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_NEWS_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (NEWS) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_NEWS_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_NEWS_IMAGE_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (NEWSIMAGE) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_NEWS_IMAGE_TABLE_SQL );
        return;
    }

    if ( ! query.exec( SETUP_STATISTIC_CLIENT_CACHE ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (STATISTIC_CLIENT_CACHE) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_STATISTIC_CLIENT_CACHE );
        return;
    }

    if ( ! query.exec( SETUP_CONTENT_TABLE_SQL ) ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Setup Schema (CONTENT) failed: " + conn.lastError().text() );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "SQL: " + SETUP_CONTENT_TABLE_SQL );
        return;
    }
}

void MediaDB::ensureDefaultData() {
    //Ensure AUDIO Media Type
    ensureMediaType( MEDIA_TYPE_AUDIO, Constants::MEDIA_TYPE_AUDIO_NAME );
    //Ensure VIDEO Media Type
    ensureMediaType( MEDIA_TYPE_VIDEO, Constants::MEDIA_TYPE_VIDEO_NAME );
    //Ensure IMAGE Media Type
    ensureMediaType( MEDIA_TYPE_IMAGE, Constants::MEDIA_TYPE_IMAGE_NAME );
    //Ensure TV-CHANNEL Media Type
    ensureMediaType( MEDIA_TYPE_TV, Constants::MEDIA_TYPE_TV_NAME );
    //Ensure DOCUMENT Media Type
    ensureMediaType( MEDIA_TYPE_DOCUMENT, Constants::MEDIA_TYPE_DOCUMENT_NAME );
    //SystemMetaData DBVersion
    ensureSystemMetaData( DB_VERSION_METADATA_DATAKEY, DB_VERSION_METADATA_DATAVALUE );
    //SystemMetaData PlayerUUID
    ensureSystemMetaData( PLAYER_UUID_METADATA_DATAKEY, RedBullPlayer::Tools::UuidCreator::create().toString() );
    const QList<IMediaType*>& types = mediaTypes();

    for( int i = 0; i < types.count(); i++ ) {
        if ( types.at( i )->guid() == MEDIA_TYPE_VIDEO ) {
            _videoType = types.at( i );
        }

        if ( types.at( i )->guid() == MEDIA_TYPE_AUDIO ) {
            _audioType = types.at( i );
        }

        if ( types.at( i )->guid() == MEDIA_TYPE_IMAGE ) {
            _imageType = types.at( i );
        }

        if ( types.at( i )->guid() == MEDIA_TYPE_TV ) {
            _tvType = types.at( i );
        }

        if ( types.at( i )->guid() == MEDIA_TYPE_DOCUMENT ) {
            _documentType = types.at( i );
        }
    }
}

void MediaDB::ensureMediaType( QString id, QString name ) {
    QSqlTableModel model;
    model.setTable( MEDIA_TYPE_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( id ) );
    model.select();

    if ( model.rowCount() == 0 ) {
        QSqlQuery query;
        query.prepare( INSERT_MEDIA_TYPE_TEMPLATE );
        query.bindValue( ":GUID", id );
        query.bindValue( ":Name", name );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "SQL Statement failed: " + query.executedQuery() );
        }
    }

    model.clear();
}

void MediaDB::ensureSystemMetaData( QString key, QString value ) {
    QSqlTableModel model;
    model.setTable( SYSTEMMETADATA_TABLE_NAME );
    model.setFilter( QString( "DataKey='%1'" ).arg( key ) );
    model.select();

    if ( model.rowCount() == 0 ) {
        QSqlQuery query;
        query.prepare( INSERT_SYSTEMMETADATA_TEMPLATE );
        query.bindValue( ":DataKey", key );
        query.bindValue( ":DataValue", value );

        if ( !query.exec() ) {
            SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "SQL Statement failed: " + query.executedQuery() );
        }
    }

    model.clear();
}

const QList<IMediaType*>& MediaDB::mediaTypes() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );

    if ( ! _mediaTypes ) {
        _mediaTypes = new QList<IMediaType*>();
        QSqlTableModel model;
        model.setTable( MEDIA_TYPE_TABLE_NAME );
        model.select();

        for( int i = 0; i < model.rowCount(); i++ ) {
            QUuid guid = model.record( i ).value( "GUID" ).toString();
            QString name = model.record( i ).value( "Name" ).toString();
            SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Setup media type: " + guid.toString() + " " + name );
            _mediaTypes->append( new MediaType( guid, name ) );
        }
    }

    return *_mediaTypes;
}

QSqlDatabase MediaDB::db() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    return QSqlDatabase::database();
}

QString MediaDB::databaseName() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    return db().databaseName();
}
void MediaDB::exportDataToXml( QString fileName ) {
    MediaDBExport::exportToXml( fileName );
}


