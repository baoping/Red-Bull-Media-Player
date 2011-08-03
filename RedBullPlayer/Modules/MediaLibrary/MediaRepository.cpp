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
#include "MediaRepository.h"

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
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/FileSystem/PathResolver.h"
#include "../../Container/Constants.h"

#include "MediaDB_sql.h"
#include "Tag.h"
#include "Media.h"
#include "MediaType.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

MediaRepository::MediaRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    _mediaDb = mediaDb;
}

MediaRepository::~MediaRepository() {
}

IMedia* MediaRepository::createNewMedia( const QUuid& guid, const QString& fileName, const IMediaType& mediaType ) {
    QMutexLocker locker( &_createNewMediaMutex );
    return new Media( guid, fileName, mediaType );
}

IMedia* MediaRepository::createMediaFromSQLresult( QSqlRecord record ) {
    QMutexLocker locker( &_createMediaFromSQLresultMutex );
    QUuid guid = record.value( "GUID" ).toString();
    QString fileName = record.value( "FileName" ).toString();
    QUuid mediaTypeID = record.value( "MediaType" ).toString();
    IMediaType* type = 0;

    //Set the media type
    if ( mediaTypeID == MEDIA_TYPE_VIDEO ) {
        type = _mediaDb->videoType();
    } else  if ( mediaTypeID == MEDIA_TYPE_AUDIO ) {
        type = _mediaDb->audioType();
    } else  if ( mediaTypeID == MEDIA_TYPE_IMAGE ) {
        type = _mediaDb->imageType();
    } else  if ( mediaTypeID == MEDIA_TYPE_TV ) {
        type = _mediaDb->tvChannelType();
    } else if ( mediaTypeID == MEDIA_TYPE_DOCUMENT ) {
        type = _mediaDb->documentType();
    } else {
        SimpleLogger::instance()->fatal( Constants::LOGGER_MEDIALIBRARY, "Invalid MediaType ID found in db" );
    }

    int length = record.value( "Length" ).toInt();
    int trackNumber = record.value( "TrackNumber" ).toInt();
    QString Title = record.value( "Title" ).toString();
    QString Artist = record.value( "Artist" ).toString();
    QString Genre = record.value( "Genre" ).toString();
    QString Copyright = record.value( "Copyright" ).toString();
    QString Album = record.value( "Album" ).toString();
    QString Description = record.value( "Description" ).toString();
    QString Rating = record.value( "Rating" ).toString();
    QString Setting = record.value( "Setting" ).toString();
    QString Url = record.value( "Url" ).toString();
    QString Language = record.value( "Language" ).toString();
    QString Publisher = record.value( "Publisher" ).toString();
    QString EncodedBy = record.value( "EncodedBy" ).toString();
    QString ArtworkUrl = record.value( "ArtworkUrl" ).toString();
    QString TrackID = record.value( "TrackID" ).toString();
    bool isNew = record.value( "IsNew" ).toBool();
    uint fileSize = record.value( "FileSize" ).toUInt();
    int startTime = record.value( "StartTime" ).toInt();
    QString startAnimationFile = record.value( "StartAnimation" ).toString();
    int animationFrameCount = record.value( "AnimationFrameCount" ).toInt();
    QDateTime date = QDateTime::fromTime_t( record.value( "Date" ).toUInt() );
    QDateTime CreatedAt = QDateTime::fromTime_t( record.value( "CreatedAt" ).toUInt() );
    IMedia* media = new Media( guid,
                               fileName,
                               *type,
                               length,
                               trackNumber,
                               Title,
                               Artist,
                               Genre,
                               Copyright,
                               Album,
                               Description,
                               Rating,
                               date,
                               Setting,
                               Url,
                               Language,
                               Publisher,
                               EncodedBy,
                               ArtworkUrl,
                               TrackID,
                               isNew,
                               fileSize,
                               startTime,
                               CreatedAt,
                               startAnimationFile,
                               animationFrameCount
                             );

    if ( ! record.value( "CoverImage" ).isNull() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Found CoverImage data in DB" );
        QImage img;
        img.loadFromData( record.value( "CoverImage" ).toByteArray() );

        if ( img.isNull() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Reading Image from db failed!" );
        }

        media->setCoverImage( img );
    }

    return media;
}

bool MediaRepository::saveMedia( const IMedia& media, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_saveMediaMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "media Item 4 Save : filename = %1 ; title = %2" ).arg( media.fileName() ).arg( media.title() ) );
    IMedia* m = getMediaByGuid( media.guid() );

    if ( m ) {
        QString tilte = media.title();

        if  ( ! media.title().isNull() && QFileInfo(  media.title() ) .exists() ) {
            tilte = QFileInfo(  media.title() ) .fileName();
        }

        QSqlQuery query;
        query.prepare( UPDATE_MEDIA_TEMPLATE );
        query.bindValue( ":GUID", media.guid().toString() );
        query.bindValue( ":Length", media.length() );
        query.bindValue( ":Title", tilte );
        query.bindValue( ":Artist", media.artist() );
        query.bindValue( ":Genre", media.genre() );
        query.bindValue( ":Copyright", media.copyright() );
        query.bindValue( ":Album", media.album() );
        query.bindValue( ":TrackNumber", media.trackNumber() );
        query.bindValue( ":Description", media.description() );
        query.bindValue( ":Rating", media.rating() );
        query.bindValue( ":Date", media.date().toTime_t() );
        query.bindValue( ":Setting", media.setting() );
        query.bindValue( ":Url", media.url() );
        query.bindValue( ":Language", media.language() );
        query.bindValue( ":Publisher", media.publisher() );
        query.bindValue( ":EncodedBy", media.encodedBy() );
        query.bindValue( ":ArtworkUrl", media.artworkUrl() );
        query.bindValue( ":TrackID", media.trackID() );
        query.bindValue( ":IsNew", media.isNew() );
        query.bindValue( ":FileSize", media.fileSize() );
        query.bindValue( ":StartTime", media.startTime() );
        query.bindValue( ":StartAnimation", media.startAnimationFile() );
        query.bindValue( ":AnimationFrameCount", media.animationFrameCount() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "MediaRepository: " + query.lastError().text() );
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "MediaRepository::saveMedia failed! Insert sql: " + query.executedQuery() );
            error->setErrorMessage( "Could not update media", Error::Normal );
            return false;
        }
    } else {
        QString tilte = media.title();

        if  ( ! media.title().isNull() && QFileInfo(  media.title() ) .exists() ) {
            tilte = QFileInfo(  media.title() ) .fileName();
        }

        QSqlQuery query;
        query.prepare( INSERT_MEDIA_TEMPLATE );
        query.bindValue( ":GUID", media.guid().toString() );
        query.bindValue( ":FileName", media.resolvableFileName() );
        query.bindValue( ":MediaType", media.mediaType().guid().toString() );
        query.bindValue( ":Length", media.length() );
        query.bindValue( ":Title", tilte );
        query.bindValue( ":Artist", media.artist() );
        query.bindValue( ":Genre", media.genre() );
        query.bindValue( ":Copyright", media.copyright() );
        query.bindValue( ":Album", media.album() );
        query.bindValue( ":TrackNumber", media.trackNumber() );
        query.bindValue( ":Description", media.description() );
        query.bindValue( ":Rating", media.rating() );
        query.bindValue( ":Date", media.date().toTime_t() );
        query.bindValue( ":Setting", media.setting() );
        query.bindValue( ":Url", media.url() );
        query.bindValue( ":Language", media.language() );
        query.bindValue( ":Publisher", media.publisher() );
        query.bindValue( ":EncodedBy", media.encodedBy() );
        query.bindValue( ":ArtworkUrl", media.artworkUrl() );
        query.bindValue( ":TrackID", media.trackID() );
        query.bindValue( ":IsNew", media.isNew() );
        query.bindValue( ":FileSize", media.fileSize() );
        query.bindValue( ":StartTime", media.startTime() );
        query.bindValue( ":StartAnimation", media.startAnimationFile() );
        query.bindValue( ":CreatedAt", QDateTime::currentDateTime().toTime_t() );
        query.bindValue( ":AnimationFrameCount", media.animationFrameCount() );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagRepository::saveMedia failed! Insert sql: " + query.executedQuery() );
            error->setErrorMessage( "Could not save media", Error::Normal );
            return false;
        }
    }

    //Save CoverImage
    if ( ! media.coverImage().isNull() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Writing Image data to database" );
        QByteArray ba;
        QBuffer buf( &ba );
        media.coverImage().save( &buf, "PNG" );
        QSqlQuery query;
        query.prepare( UPDATE_MEDIA_COVER_TEMPLATE );
        query.bindValue( ":GUID", media.guid().toString() );
        query.bindValue( ":CoverImage", QVariant( ba ) );

        if ( ! query.exec() ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "MediaRepository::saveMedia failed! Insert sql: " + query.executedQuery() );
            error->setErrorMessage( "Could not save media CoverImage", Error::Normal );
            return false;
        }
    }

    return true;
}

bool MediaRepository::deleteMedia( const IMedia& media, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_deleteMediaMutex );
    QSqlQuery query;
    query.prepare( DELETE_MEDIATAG_BY_MEDIA_TEMPLATE );
    query.bindValue( ":Media_GUID", media.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete Media", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    query.clear();
    query.prepare( DELETE_MEDIA_TEMPLATE );
    query.bindValue( ":GUID", media.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete Media", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    return true;
}


IMedia* MediaRepository::getMediaByGuid( QUuid guid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getMediaByGuidMutex );
    QSqlTableModel model;
    model.setTable( MEDIA_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( guid.toString() ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createMediaFromSQLresult( model.record( 0 ) );
    }

    return 0;
}

IMedia* MediaRepository::getMediaByFileName( QString fileName ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QString queryFileName = PathResolver::createResolvablePath(
                                fileName,
                                ConfigManager::instance()->getMediaDirectory(),
                                Constants::MEDIADIRECTORY_PATH_PLACEHOLDER  );
    QMutexLocker locker( &_getMediaByFileNameMutex );
    QSqlTableModel model;
    model.setTable( MEDIA_TABLE_NAME );
    model.setFilter( QString( "FileName='%1'" ).arg( queryFileName ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createMediaFromSQLresult( model.record( 0 ) );
    }

    return 0;
}

QList<IMedia*>* MediaRepository::allMedia() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_allMediaMutex );
    QList<IMedia*>* ret = new QList<IMedia*>();
    QSqlTableModel model;
    model.setTable( MEDIA_TABLE_NAME );
    model.select();

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createMediaFromSQLresult( model.record( i ) ) );
    }

    return ret;
}

QList<QPair<QUuid, QString>*>* MediaRepository::allMediaInUIOrder() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_allMediaInUIOrderMutex );
    QList<QPair<QUuid, QString>*>* ret = new QList<QPair<QUuid, QString>*>();
    QSqlQuery query;
    query.prepare( SELECT_ALL_MEDIA_IN_UI_ORDER );
    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        QPair<QUuid, QString>* p = new QPair<QUuid, QString>();
        p->first = QUuid( model.record( i ).value( "GUID" ).toString() );
        QString resolvedFileName = PathResolver::resolvePath(
                                       model.record( i ).value( "FileName" ).toString(),
                                       ConfigManager::instance()->getMediaDirectory(),
                                       Constants::MEDIADIRECTORY_PATH_PLACEHOLDER );
        p->second = resolvedFileName;
        ret->append( p );
    }

    return ret;
}

QList<QUuid>* MediaRepository::getMediaUsingDBSearch( QString queryString ) {
    QMutexLocker locker( &_getMediaUsingDBSearchMutex );
    QList<QUuid>* ret = new QList<QUuid>();
    QString preparedQuery =  QString( "%1%" ) .arg( queryString );
    QSqlQuery query;
    query.prepare( SELECT_MEDIA_FOR_QUICK_DB_SEARCH );
    query.bindValue( ":Query", preparedQuery );

    if ( ! query.exec() ) {
        qWarning() << "Query: " << query.executedQuery();
        qFatal( "Invalid query" );
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Executed Query: %1" ).arg( query.executedQuery() ) );
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( QUuid( model.record( i ).value( "GUID" ).toString() ) );
    }

    return ret;
}

QList<IMedia*>* MediaRepository::getMediaByMediaType( const IMediaType& mediaType ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getMediaByMediaTypeMutex );
    QList<IMedia*>* ret = new QList<IMedia*>();
    QSqlTableModel model;
    model.setTable( MEDIA_TABLE_NAME );
    model.setFilter( QString( "MediaType='%1'" ).arg( mediaType.guid().toString() ) );
    model.select();

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createMediaFromSQLresult( model.record( i ) ) );
    }

    return ret;
}

QList<IMedia*>* MediaRepository::getMediaByTag( const ITag& tag ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_getMediaByTagMutex );
    QList<IMedia*>* ret = new QList<IMedia*>();
    QSqlQuery query;
    query.prepare( SELECT_MEDIA_BY_TAG_TEMPLATE );
    query.bindValue( ":Tag_GUID", tag.guid().toString() );
    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createMediaFromSQLresult( model.record( i ) ) );
    }

    return ret;
}

bool MediaRepository::addTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_addTagMutex );
    QSqlQuery query;
    query.prepare( INSERT_MEDIATAG_TEMPLET );
    query.bindValue( ":Media_GUID", media.guid().toString() );
    query.bindValue( ":Tag_GUID", tag.guid().toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagRepository::addTag failed! insert sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not add Tag", Error::Normal );
        return false;
    }

    return true;
}

bool MediaRepository::removeTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_removeTagMutex );
    QSqlQuery query;
    query.prepare( DELETE_MEDIATAG_TEMPLET );
    query.bindValue( ":Media_GUID", media.guid().toString() );
    query.bindValue( ":Tag_GUID", tag.guid().toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagRepository::removeTag failed! delete sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not remove Tag", Error::Normal );
        return false;
    }

    return true;
}

bool MediaRepository::isOnlineContent( QUuid guid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QMutexLocker locker( &_isOnlineContentMutex );
    QSqlQuery query;
    query.prepare( SELECT_ISONLINECONTENT_SQL );
    query.bindValue( ":MEDIA_GUID", guid.toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->error( "MediaRepository", "Error executing sql: " + query.executedQuery() );
        return false;
    }

    QSqlQueryModel model;
    model.setQuery( query );

    if (  model.rowCount() == 0 ) {
        qWarning() << "Invalid Query!";
        return false;
    }

    return  model.record( 0 ).value( "IsOnlineContent" ).toInt() > 0;
}
