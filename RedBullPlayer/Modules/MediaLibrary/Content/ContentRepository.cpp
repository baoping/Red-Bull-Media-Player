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
#include "ContentRepository.h"

// Qt includes
#include <QDebug>
#include <QImage>
#include <QMutexLocker>
#include <QString>

// Project includes
#include "Content_sql.h"
#include "Content.h"

//logging
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Container/Constants.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

ContentRepository::ContentRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    _mediaDb = mediaDb;
}

ContentRepository::~ContentRepository() { }

IContent* ContentRepository::createContentFromSqlResult( QSqlRecord record ) {
    QMutexLocker locker( &_createContentFromSqlResultMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QString guid = record.value( "GUID" ).toString();
    QString name = record.value( "Name" ).toString();
    QString fileName = record.value( "FileName" ).toString();
    QString downloadUrl = record.value( "DownloadUrl" ).toString();
    IContent* content = new Content( guid, name, fileName, downloadUrl );

    if ( !record.value( "Media_GUID" ).isNull() ) {
        QUuid mediaGuid = record.value( "Media_GUID" ).toString();
        content->setMediaGuid( mediaGuid );
    }

    uint fileSize = record.value( "FileSize" ).toUInt();
    QDateTime date = QDateTime::fromTime_t( record.value( "Date" ).toUInt() );
    QString state = record.value( "State" ).toString();
    content->setFileSize( fileSize );
    content->setDate( date );
    content->setState( state );
    return content;
}

IContent* ContentRepository::createContent( const QString& guid,
        const QString& name,
        const QString& fileName,
        const QString& downloadUrl ) {
    QMutexLocker locker( &_createContentMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    IContent* content = new Content( guid, name, fileName, downloadUrl );
    content->setState( Constants::CONTENT_NEW );
    content->setDate( QDateTime::currentDateTime() );
    return content;
}

bool ContentRepository::saveContent( const IContent& content, Error* error ) {
    QMutexLocker locker( &_saveContentMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Error err;
    IContent* c = getContentByGuid( content.guid() );

    if ( c ) {
        QSqlQuery query;

        if ( !content.mediaGuid().isNull() ) {
            query.prepare( UPDATE_CONTENT_TEMPLATE );
            query.bindValue( ":Media_GUID", content.mediaGuid().toString() );
        } else {
            query.prepare( UPDATE_CONTENT_TEMPLATE_EXCLUDING_MEDIA_GUID );
        }

        query.bindValue( ":GUID", content.guid() );
        query.bindValue( ":FileName", content.fileName() );
        query.bindValue( ":Name", content.name() );
        query.bindValue( ":DownloadUrl", content.downloadUrl() );
        query.bindValue( ":FileSize", content.fileSize() );
        query.bindValue( ":Date", content.date().toTime_t() );
        query.bindValue( ":State", content.state() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( "ContentRepository", "Could not update Content: " + query.executedQuery() );
            qDebug() << query.lastError().text();
            error->setErrorMessage( "Could not update Content", Error::Normal );
            return false;
        }

        return true;
    } else {
        QSqlQuery query;

        if ( !content.mediaGuid().isNull() ) {
            query.prepare( INSERT_CONTENT_TEMPLATE );
            query.bindValue( ":Media_GUID", content.mediaGuid().toString() );
        } else {
            query.prepare( INSERT_CONTENT_TEMPLATE_EXCLUDING_MEDIA_GUID );
        }

        query.bindValue( ":GUID", content.guid() );
        query.bindValue( ":FileName", content.fileName() );
        query.bindValue( ":Name", content.name() );
        query.bindValue( ":DownloadUrl", content.downloadUrl() );
        query.bindValue( ":FileSize", content.fileSize() );
        query.bindValue( ":Date", content.date().toTime_t() );
        query.bindValue( ":State", content.state() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( "ContentRepository", "Could not insert Content: " + query.executedQuery() );
            error->setErrorMessage( "Could not insert PodCastMedia", Error::Normal );
            return false;
        }

        return true;
    }
}

IContent* ContentRepository::getContentByGuid( QString guid ) {
    QMutexLocker locker( &_getContentByGuidMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QSqlTableModel model;
    model.setTable( CONTENT_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( guid ) );
    model.select();

    if ( model.rowCount() >= 1 ) {
        return createContentFromSqlResult( model.record( 0 ) );
    }

    return 0;
}

IContent* ContentRepository::getContentByMediaGuid( QString mediaGuid ) {
    QMutexLocker locker( &_getContentByMediaGuidMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QSqlTableModel model;
    model.setTable( CONTENT_TABLE_NAME );
    model.setFilter( QString( "MEDIA_GUID='%1'" ).arg( mediaGuid ) );
    model.select();

    if ( model.rowCount() >= 1 ) {
        return createContentFromSqlResult( model.record( 0 ) );
    }

    return 0;
}

QList<IContent*>* ContentRepository::allContent() {
    QMutexLocker locker( &_allContentMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QList<IContent*>* ret = new QList<IContent*>();
    QSqlTableModel model;
    model.setTable( CONTENT_TABLE_NAME );
    model.select();

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createContentFromSqlResult( model.record( i ) ) );
    }

    return ret;
}

QList<IContent*>* ContentRepository::allContentByState( QString state ) {
    QMutexLocker locker( &_allContentByStateMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QList<IContent*>* ret = new QList<IContent*>();
    QSqlQuery query;
    query.prepare( SELECT_CONTENT_BY_STATE_TEMPLATE );
    query.bindValue( ":State", state );
    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );
    qDebug() << "ContentRepository => AllContentByState(" << state << ").Count=" << model.rowCount();

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createContentFromSqlResult( model.record( i ) ) );
    }

    return ret;
}

bool ContentRepository::deleteContent( const IContent& content, Error* error ) {
    QMutexLocker locker( &_deleteContentMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QSqlQuery query;
    query.prepare( DELETE_CONTENT_TEMPLATE );
    query.bindValue( ":GUID", content.guid() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( "ContentRepository", "Could not delete content: " + query.lastError().text() );
        error->setErrorMessage( "Could not delete Content", Error::Normal );
        return false;
    }

    return true;
}

