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
#include "TagRepository.h"

// Qt includes
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QMutexLocker>

// Project includes
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

#include "MediaDB_sql.h"
#include "Tag.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

TagRepository::TagRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( mediaDb != 0 );
    _mediaDb = mediaDb;
}

ITag* TagRepository::createTagFromSQLresult( QSqlRecord record ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_createTagFromSQLresultMutex );
    QUuid guid = record.value( "GUID" ).toString();
    QString name = record.value( "Name" ).toString();
    bool builtIn  = record.value( "BuiltIn" ).toBool();
    return new Tag( guid, name, builtIn );
}

ITag* TagRepository::getTagByGuid( QUuid guid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getTagByGuidMutex );
    QSqlTableModel model;
    model.setTable( TAG_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( guid.toString() ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createTagFromSQLresult( model.record( 0 ) );
    }

    return 0;
}

ITag* TagRepository::getTagByName( QString name ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getTagByNameMutex );
    QSqlTableModel model;
    model.setTable( TAG_TABLE_NAME );
    model.setFilter( QString( "Name='%1'" ).arg( name ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createTagFromSQLresult( model.record( 0 ) );
    }

    return 0;
}
QList<ITag*>* TagRepository::getTagsByNameQuery( QString nameQuery ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getTagsByNameQueryMutex );
    QList<ITag*>* ret = new QList<ITag*>();
    QSqlTableModel model;
    model.setTable( TAG_TABLE_NAME );
    model.setFilter( QString( "Name LIKE '%1'" ).arg( nameQuery ) );
    model.select();

    for ( int i = 0; i < model.rowCount(); i++ ) {
        ITag* t = createTagFromSQLresult( model.record( i ) );
        ret->append( t );
    }

    return ret;
}

QList<ITag*>* TagRepository::getTagsByMedia( const IMedia& media ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getTagsByMediaMutex );
    QList<ITag*>* ret = new QList<ITag*>();
    QSqlQuery query;
    query.prepare( SELECT_TAG_BY_MEDIA_TEMPLATE );
    query.bindValue( ":Media_GUID", media.guid().toString() );
    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createTagFromSQLresult( model.record( i ) ) );
    }

    return ret;
}

bool TagRepository::saveTag( const ITag& tag, Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_saveTagMutex );
    ITag* sameNameTag = getTagByName( tag.name() );

    if ( sameNameTag ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, QString( "Tag with name %1 already exists." ).arg( tag.name() ) );
        error->setErrorMessage( tr( "Tag with name %1 already exists." ).arg( tag.name() ), Error::Normal );
        return false;
    }

    ITag* t = getTagByGuid( tag.guid() );

    if ( t ) {
        QSqlQuery query;
        query.prepare( UPDATE_TAG_TEMPLATE );
        query.bindValue( ":GUID", tag.guid().toString() );
        query.bindValue( ":Name", tag.name() );
        query.bindValue( ":BuiltIn", ( int )tag.builtIn() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagRepository::saveTag failed! Update sql: " + query.executedQuery() );
            return false;
        }

        return true;
    } else {
        QSqlQuery query;
        query.prepare( INSERT_TAG_TEMPLATE );
        // :GUID, :Name, :BuiltIn
        query.bindValue( ":GUID", tag.guid().toString() );
        query.bindValue( ":Name", tag.name() );
        query.bindValue( ":BuiltIn", ( int )tag.builtIn() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagRepository::saveTag failed! Insert sql: " + query.executedQuery() );
            return false;
        }

        return true;
    }
}

bool TagRepository::deleteTag( const ITag& tag, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_deleteTagMutex );
    QSqlQuery query;
    query.prepare( DELETE_MEDIATAG_BY_TAG_TEMPLATE );
    query.bindValue( ":Tag_GUID", tag.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete MediaTags", Error::Normal );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    query.clear();
    query.prepare( DELETE_TAG_TEMPLATE );
    query.bindValue( ":GUID", tag.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete Tag", Error::Normal );
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    return true;
}

QList<ITag*>* TagRepository::tags() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_tagsMutex );
    QList<ITag*>* ret = new QList<ITag*>();
    QSqlTableModel model;
    model.setTable( TAG_TABLE_NAME );
    model.select();

    for ( int i = 0; i < model.rowCount(); i++ ) {
        ITag* t = createTagFromSQLresult( model.record( i ) );
        ret->append( t );
    }

    return ret;
}
