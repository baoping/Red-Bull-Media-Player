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
#include "NewsRepository.h"

// Qt includes
#include <QDebug>
#include <QImage>
#include <QMutexLocker>
#include <QString>


#include "../MediaDB_sql.h"
#include "News.h"
#include "NewsImage.h"

//logging
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Container/Constants.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


NewsRepository::NewsRepository( MediaDB* mediaDb, QObject* parent ) : QObject( parent ) {
    _mediaDb = mediaDb;
}

NewsRepository::~NewsRepository() {
}

INews* NewsRepository::createNews( const QUuid& guid, const QString& title ) {
    QMutexLocker locker( &_createNewsMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Create new News (%1)." ).arg( guid ) );
    return new News( guid, title );
}

INewsImage* NewsRepository::createNewsImage( const QUuid& newsGuid, const QUuid& guid, const QString& fileName ) {
    QMutexLocker locker( &_createNewsImageMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Create new NewsImage (%1) for News (%2)." ).arg( guid ).arg( newsGuid ) );
    return new NewsImage( newsGuid, guid, fileName );
}

bool NewsRepository::saveNews( const INews& news, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_saveNewsMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Save News (%1)." ).arg( news.guid().toString() ) );

    if ( !news.isValid() ) {
        error->reset();
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, QString( "Save News (%1) failed due to null or empty Title." ).arg( news.guid().toString() ) );
        return false;
    }

    bool exists = newsExists( news.guid(), error );

    if ( error->errorType() != Error::NoError )
        return false;

    error->reset();
    return ( exists ) ? updateNews( news, error ) : insertNews( news, error );
}

bool NewsRepository::saveNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_saveNewsImageMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Save NewsImage (%1)." ).arg( newsImage.guid().toString() ) );
    bool exists = newsImageExists( newsImage.guid(), error );

    if ( error->errorType() != Error::NoError )
        return false;

    error->reset();
    bool success =  ( exists ) ? updateNewsImage( newsImage, error ) : insertNewsImage( newsImage, error );

    if ( !success ) {
        return false;
    } else {
        error->reset();

        //update image
        if ( ! newsImage.image().isNull() ) {
            QByteArray byteArray;
            QBuffer buffer( &byteArray );
            newsImage.image().save( &buffer, "PNG" );
            QSqlQuery query;
            query.prepare( UPDATE_NEWS_IMAGE_IMAGE_TEMPLATE );
            query.bindValue( ":GUID", newsImage.guid().toString() );
            query.bindValue( ":Image", QVariant( byteArray ) );

            if ( ! query.exec() ) {
                SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "NewsRepository::saveNewsImage failed! Insert sql: " + query.executedQuery() );
                error->setErrorMessage( "Could not save news image.", Error::Normal );
                return false;
            }
        }

        return true;
    }
}

QList<INews*>* NewsRepository::allNews( bool orderByPubDate ) {
    QMutexLocker locker( &_allNewsMutex );
    QList<INews*>* ret = new QList<INews*>();
    QSqlQuery query;

    if ( orderByPubDate ) {
        query.prepare( SELECT_ALL_NEWS_ORDER_BY_PUBDATE_SQL );
    } else {
        query.prepare( SELECT_ALL_NEWS_SQL );
    }

    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createNewsFromSQLResult( model.record( i ) ) );
    }

    return ret;
}

QList<INewsImage*>* NewsRepository::allNewsImages( QUuid newsGuid ) {
    QMutexLocker locker( &_allNewsImagesMutex );
    QList<INewsImage*>* ret = new QList<INewsImage*>();
    QSqlQuery query;
    query.prepare( SELECT_NEWS_IMAGES_BY_NEWS_GUID_SQL );
    query.bindValue( ":News_GUID", newsGuid.toString() );
    query.exec();
    QSqlQueryModel model;
    model.setQuery( query );

    for( int i = 0; i < model.rowCount(); i++ ) {
        ret->append( createNewsImageFromSQLResult( model.record( i ) ) );
    }

    return ret;
}

bool NewsRepository::anyNewsImageExists( QUuid newsGuid ) {
    QMutexLocker locker( &_newsImageExistsMutex );
    Error* error = new Error();
    QSqlQuery query;
    query.prepare( SELECT_NEWS_IMAGES_COUNT_BY_NEWS_GUID_SQL );
    query.bindValue( ":News_GUID", newsGuid.toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->error( "MediaRepository", "Error executing sql: " + query.executedQuery() );
        error->setErrorMessage( QString( "Could not execute sql: %1." ).arg( query.executedQuery() ), IError::Normal );
        return false;
    }

    Error::raise( error, true );
    delete error;
    QSqlQueryModel model;
    model.setQuery( query );

    if (  model.rowCount() == 0 ) {
        qWarning() << "Invalid Query!";
        return false;
    }

    return ( model.record( 0 ).value( "c" ).toInt() > 0 );
}

INews* NewsRepository::getNewsByGuid( QUuid guid ) {
    QMutexLocker locker( &_getNewsByGuidMutex );
    QSqlTableModel model;
    model.setTable( NEWS_TABLE_NAME );
    model.setFilter( QString( "GUID='%1'" ).arg( guid.toString() ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createNewsFromSQLResult( model.record( 0 ) );
    }

    return 0;
}

INews* NewsRepository::getNewsByExternalGuid( QString externalGuid ) {
    QMutexLocker locker( &_getNewsByGuidMutex );
    QSqlTableModel model;
    model.setTable( NEWS_TABLE_NAME );
    model.setFilter( QString( "External_GUID='%1'" ).arg( externalGuid ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createNewsFromSQLResult( model.record( 0 ) );
    }

    return 0;
}

INewsImage* NewsRepository::getNewsImageByFileName( QUuid newsGuid, QString fileName ) {
    QMutexLocker locker( &_getNewsImageByFileNameMutex );
    QSqlTableModel model;
    model.setTable( NEWS_IMAGE_TABLE_NAME );
    model.setFilter( QString( "News_GUID = '%1' and FileName='%2'" ).arg( newsGuid.toString() ).arg( fileName ) );
    model.select();

    if ( model.rowCount() == 1 ) {
        return createNewsImageFromSQLResult( model.record( 0 ) );
    }

    return 0;
}

void NewsRepository::clearCache( RedBullPlayer::Container::Error *error ) {
    QMutexLocker locker( &_deleteNewsMutex );
    QSqlQuery query;
    query.prepare( CLEAR_NEWS_IMAGE_CACHE );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not clear NewsImage cache!", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return;
    }

    query.clear();
    query.prepare( CLEAR_NEWS_CACHE );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not clear News cache!", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return;
    }

    query.clear();
}

bool NewsRepository::deleteNews( const INews& news, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_deleteNewsMutex );
    QSqlQuery query;
    query.prepare( DELETE_NEWS_IMAGE_TEMPLATE );
    query.bindValue( ":News_GUID", news.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete NewsImage", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    query.clear();
    query.prepare( DELETE_NEWS_TEMPLATE );
    query.bindValue( ":GUID", news.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete Media", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    return true;
}

bool NewsRepository::deleteNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_deleteNewsImageMutex );
    QSqlQuery query;
    query.prepare( DELETE_NEWS_IMAGE_TEMPLATE );
    query.bindValue( ":GUID", newsImage.guid().toString() );

    if ( ! query.exec() ) {
        error->setErrorMessage( "Could not delete NewsImage", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY,
                                        "Failed delete sql: " + query.executedQuery() + " sql error: " + query.lastError().text() );
        return false;
    }

    return true;
}

INews* NewsRepository::createNewsFromSQLResult( QSqlRecord record ) {
    QMutexLocker locker( &_createNewsFromSQLResultMutex );
    INews* news = new News( record.value( "GUID" ).toString(),
                            record.value( "Title" ).toString() );
    news->setAuthor( record.value( "Author" ).toString() );
    news->setCategory( record.value( "Category" ).toString() );
    news->setDescription( record.value( "Description" ).toString() );
    news->setContent( record.value( "Content" ).toString() );
    news->setExternalGuid( record.value( "External_GUID" ).toString() );
    news->setLink( record.value( "Link" ).toString() );
    news->setPubDate( QDateTime::fromTime_t( record.value( "PubDate" ).toInt() ) );
    return news;
}

INewsImage* NewsRepository::createNewsImageFromSQLResult( QSqlRecord record ) {
    QMutexLocker locker( &_createNewsImageFromSQLResultMutex );
    INewsImage* newsImage = new NewsImage( record.value( "News_GUID" ).toString(),
                                           record.value( "GUID" ).toString(),
                                           record.value( "FileName" ).toString() );
    newsImage->setHeight( record.value( "Height" ).toInt() );
    newsImage->setWidth( record.value( "Width" ).toInt() );
    newsImage->setSize( record.value( "Size" ).toInt() );

    if ( ! record.value( "Image" ).isNull() ) {
        QImage img;
        img.loadFromData( record.value( "Image" ).toByteArray() );

        if ( !img.isNull() ) {
            newsImage->setImage( img );
        }
    }

    return newsImage;
}

bool NewsRepository::newsExists( const QUuid& guid, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_newsExistsMutex );
    QSqlQuery query;
    query.prepare( SELECT_NEWS_COUNT_SQL );
    query.bindValue( ":GUID", guid.toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->error( "MediaRepository", "Error executing sql: " + query.executedQuery() );
        error->setErrorMessage( QString( "Could not execute sql: %1." ).arg( query.executedQuery() ), IError::Normal );
        return false;
    }

    QSqlQueryModel model;
    model.setQuery( query );

    if (  model.rowCount() == 0 ) {
        qWarning() << "Invalid Query!";
        return false;
    }

    return ( model.record( 0 ).value( "c" ).toInt() > 0 );
}

bool NewsRepository::newsImageExists( const QUuid& guid, RedBullPlayer::Container::Error* error ) {
    QMutexLocker locker( &_newsExistsMutex );
    QSqlQuery query;
    query.prepare( SELECT_NEWS_IMAGE_COUNT_SQL );
    query.bindValue( ":GUID", guid.toString() );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->error( "MediaRepository", "Error executing sql: " + query.executedQuery() );
        error->setErrorMessage( QString( "Could not execute sql: %1." ).arg( query.executedQuery() ), IError::Normal );
        return false;
    }

    QSqlQueryModel model;
    model.setQuery( query );

    if (  model.rowCount() == 0 ) {
        qWarning() << "Invalid Query!";
        return false;
    }

    return ( model.record( 0 ).value( "c" ).toInt() > 0 );
}

bool NewsRepository::updateNews( const INews& news, RedBullPlayer::Container::Error* error ) {
    QSqlQuery query = mapNewsQuery( news, UPDATE_NEWS_TEMPLATE );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "NewsRepository.updateNews failed! Insert sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not update News.", Error::Normal );
        return false;
    }

    return true;
}

bool NewsRepository::insertNews( const INews& news, RedBullPlayer::Container::Error* error ) {
    QSqlQuery query = mapNewsQuery( news, INSERT_NEWS_TEMPLATE );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "NewsRepository.insertNews failed! Insert sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not save News.", Error::Normal );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, query.lastError().text() );
        return false;
    }

    return true;
}

bool NewsRepository::updateNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error )  {
    QSqlQuery query = mapNewsImageQuery( newsImage, UPDATE_NEWS_IMAGE_TEMPLATE );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "NewsRepository.updateNewsImage failed! Insert sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not update NewsImage.", Error::Normal );
        return false;
    }

    return true;
}

bool NewsRepository::insertNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error ) {
    QSqlQuery query = mapNewsImageQuery( newsImage, INSERT_NEWS_IMAGE_TEMPLATE );

    if ( ! query.exec() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "NewsRepository.insertNewsImage failed! Insert sql: " + query.executedQuery() );
        error->setErrorMessage( "Could not save NewsImage.", Error::Normal );
        return false;
    }

    return true;
}

QSqlQuery NewsRepository::mapNewsQuery( const INews& news, QString sql ) {
    QSqlQuery query;
    /*QString log = QString(":GUID %1\n"
                  ":Title %2\n"
                  ":Link %3\n"
                  ":Content %4\n"
                  ":Author %5\n"
                  ":Category %6\n"
                  ":External_GUID %\n7"
                  ":PubDate %8\n")
            .arg(news.guid().toString())
            .arg(news.title())
            .arg(news.link())
            .arg( news.content())
            .arg(news.author())
            .arg(news.category())
            .arg(news.externalGuid())
            .arg(news.pubDate().toTime_t());
    SimpleLogger::instance()->warn(Constants::LOGGER_MEDIALIBRARY,"mapNewsQuery: " + log );*/
    query.prepare( sql );
    query.bindValue( ":GUID", news.guid().toString() );
    query.bindValue( ":Title", news.title() );
    query.bindValue( ":Link", news.link() );
    query.bindValue( ":Description", news.description() );
    query.bindValue( ":Content", news.content() );
    query.bindValue( ":Author", news.author() );
    query.bindValue( ":Category", news.category() );
    query.bindValue( ":External_GUID", news.externalGuid() );
    query.bindValue( ":PubDate", news.pubDate().toTime_t() );
    return query;
}

QSqlQuery NewsRepository::mapNewsImageQuery( const INewsImage& newsImage, QString sql ) {
    QSqlQuery query;
    query.prepare( sql );
    query.bindValue( ":GUID", newsImage.guid().toString() );
    query.bindValue( ":News_GUID", newsImage.newsGuid().toString() );
    query.bindValue( ":FileName", newsImage.fileName() );
    query.bindValue( ":Width", newsImage.width() );
    query.bindValue( ":Height", newsImage.height() );
    query.bindValue( ":Size", newsImage.size() );
    return query;
}

