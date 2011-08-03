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
#include "NewsRepositoryTest.h"

#include <QDebug>

#include "News/News.h"
#include "News/NewsImage.h"
#include "News/NewsRepository.h"
#include "MediaLibrary/IMediaDB.h"
#include "MediaDB.h"
#include "Shell.h"
#include "Error.h"
#include "UuidCreator.h"
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;

NewsRepositoryTest::NewsRepositoryTest() {
}

void NewsRepositoryTest::should_create_news() {
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( news->guid() == id );
    QVERIFY( news->title() == "TestNews" );
    QVERIFY( news->author() == "Author" );
    QVERIFY( news->category() == "Category" );
    QVERIFY( news->content() == "Content" );
    QVERIFY( news->externalGuid() == "ExternalGuid" );
    QVERIFY( news->link() == "Link" );
    QVERIFY( news->pubDate().toTime_t() == dateTime.toTime_t() );
}

void NewsRepositoryTest::should_create_news_from_news() {
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    INews* copyNews = new News( *news );
    QVERIFY( copyNews->guid() == id );
    QVERIFY( copyNews->title() == "TestNews" );
    QVERIFY( copyNews->author() == "Author" );
    QVERIFY( copyNews->category() == "Category" );
    QVERIFY( copyNews->content() == "Content" );
    QVERIFY( copyNews->externalGuid() == "ExternalGuid" );
    QVERIFY( copyNews->link() == "Link" );
    QVERIFY( copyNews->pubDate().toTime_t() == dateTime.toTime_t() );
}

void NewsRepositoryTest::should_create_newsImage() {
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    INewsImage* newsImage = new NewsImage( newsId, id, "RedBullLogo.png" );
    newsImage->setHeight( 100 );
    newsImage->setWidth( 100 );
    newsImage->setSize( 100 );
    newsImage->setImage( image );
    QVERIFY( newsImage->fileName() == "RedBullLogo.png" );
    QVERIFY( newsImage->guid() == id );
    QVERIFY( newsImage->newsGuid() == newsId );
    QVERIFY( newsImage->height() == 100 );
    QVERIFY( newsImage->height() == 100 );
    QVERIFY( newsImage->size() == 100 );
    QVERIFY( !newsImage->image().isNull() );
}

void NewsRepositoryTest::should_create_newsImage_from_newsImage() {
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    INewsImage* newsImage = new NewsImage( newsId, id, "RedBullLogo.png" );
    newsImage->setHeight( 100 );
    newsImage->setWidth( 100 );
    newsImage->setSize( 100 );
    newsImage->setImage( image );
    INewsImage* copyNewsImage = new NewsImage( *newsImage );
    QVERIFY( copyNewsImage->fileName() == "RedBullLogo.png" );
    QVERIFY( copyNewsImage->guid() == id );
    QVERIFY( copyNewsImage->newsGuid() == newsId );
    QVERIFY( copyNewsImage->height() == 100 );
    QVERIFY( copyNewsImage->height() == 100 );
    QVERIFY( copyNewsImage->size() == 100 );
    QVERIFY( !copyNewsImage->image().isNull() );
}

void NewsRepositoryTest::should_create_repository() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QVERIFY( rep != 0 );
}

void NewsRepositoryTest::should_create_news_from_repository() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = rep->createNews( id, "Title" );
    QVERIFY( news != 0 );
    QVERIFY( news->guid() == id );
    QVERIFY( news->title() == "Title" );
}

void NewsRepositoryTest::should_create_newsImage_from_repository() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = rep->createNewsImage( newsId, id, "FileName" );
    QVERIFY( newsImage != 0 );
    QVERIFY( newsImage->newsGuid() == newsId );
    QVERIFY( newsImage->guid() == id );
    QVERIFY( newsImage->fileName() == "FileName" );
}

void NewsRepositoryTest::should_save_news() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
}

void NewsRepositoryTest::should_not_save_news() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, QString::null );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( ! rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
}

void NewsRepositoryTest::should_get_news_by_guid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    INews* gotNews = rep->getNewsByGuid( id );
    QVERIFY( gotNews != 0 );
    QVERIFY( gotNews->guid() == id );
    QVERIFY( gotNews->title() == "TestNews" );
    QVERIFY( gotNews->author() == "Author" );
    QVERIFY( gotNews->category() == "Category" );
    QVERIFY( gotNews->content() == "Content" );
    QVERIFY( gotNews->externalGuid() == "ExternalGuid" );
    QVERIFY( gotNews->link() == "Link" );
    QVERIFY( gotNews->pubDate().toTime_t() == dateTime.toTime_t() );
}

void NewsRepositoryTest::should_get_news_by_external_guid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    INews* gotNews = rep->getNewsByExternalGuid( "ExternalGuid" );
    QVERIFY( gotNews != 0 );
    QVERIFY( gotNews->guid() == id );
    QVERIFY( gotNews->title() == "TestNews" );
    QVERIFY( gotNews->author() == "Author" );
    QVERIFY( gotNews->category() == "Category" );
    QVERIFY( gotNews->content() == "Content" );
    QVERIFY( gotNews->externalGuid() == "ExternalGuid" );
    QVERIFY( gotNews->link() == "Link" );
    QVERIFY( gotNews->pubDate().toTime_t() == dateTime.toTime_t() );
}

void NewsRepositoryTest::should_update_news() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    news->setAuthor( "NewAuthor" );
    news->setCategory( "NewCategory" );
    news->setContent( "NewContent" );
    news->setExternalGuid( "NewExternalGuid" );
    news->setLink( "NewLink" );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    INews* gotNews = rep->getNewsByGuid( id );
    QVERIFY( gotNews != 0 );
    QVERIFY( gotNews->guid() == id );
    QVERIFY( gotNews->title() == "TestNews" );
    QVERIFY( gotNews->author() == "NewAuthor" );
    QVERIFY( gotNews->category() == "NewCategory" );
    QVERIFY( gotNews->content() == "NewContent" );
    QVERIFY( gotNews->externalGuid() == "NewExternalGuid" );
    QVERIFY( gotNews->link() == "NewLink" );
    QVERIFY( gotNews->pubDate().toTime_t() == dateTime.toTime_t() );
}

void NewsRepositoryTest::should_delete_news() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime dateTime = QDateTime::currentDateTime();
    INews* news = new News( id, "TestNews" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( dateTime );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QVERIFY( rep->deleteNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    INews* gotNews = rep->getNewsByGuid( id );
    QVERIFY( gotNews == 0 );
}

void NewsRepositoryTest::should_get_all_news() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid firstId = RedBullPlayer::Tools::UuidCreator::create();
    INews* firstNews = new News( firstId, "FirstNews" );
    firstNews->setAuthor( "FirstAuthor" );
    firstNews->setCategory( "FirstCategory" );
    firstNews->setContent( "FirstContent" );
    firstNews->setExternalGuid( "FirstExternalGuid" );
    firstNews->setLink( "FirstLink" );
    firstNews->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *firstNews, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid secondId = RedBullPlayer::Tools::UuidCreator::create();
    INews* secondNews = new News( secondId, "SecondNews" );
    secondNews->setAuthor( "SecondAuthor" );
    secondNews->setCategory( "SecondCategory" );
    secondNews->setContent( "SecondContent" );
    secondNews->setExternalGuid( "SecondExternalGuid" );
    secondNews->setLink( "SecondLink" );
    secondNews->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *secondNews, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QList<INews*>* allNews = rep->allNews( false );
    bool containsFirst = false;
    bool containsSecond = false;
    QVERIFY( allNews->count() == 2 );

    for( int i = 0; i < allNews->count(); i++ ) {
        INews* news = allNews->at( i );

        if ( news->guid() == firstId )  {
            containsFirst = true;
            QVERIFY( news->guid() == firstId );
            QVERIFY( news->title() == "FirstNews" );
            QVERIFY( news->author() == "FirstAuthor" );
            QVERIFY( news->category() == "FirstCategory" );
            QVERIFY( news->content() == "FirstContent" );
            QVERIFY( news->externalGuid() == "FirstExternalGuid" );
            QVERIFY( news->link() == "FirstLink" );
        }

        if ( news->guid() == secondId ) {
            containsSecond = true;
            QVERIFY( news->guid() == secondId );
            QVERIFY( news->title() == "SecondNews" );
            QVERIFY( news->author() == "SecondAuthor" );
            QVERIFY( news->category() == "SecondCategory" );
            QVERIFY( news->content() == "SecondContent" );
            QVERIFY( news->externalGuid() == "SecondExternalGuid" );
            QVERIFY( news->link() == "SecondLink" );
        }
    }

    QVERIFY( containsFirst && containsSecond );
}

void NewsRepositoryTest::should_get_all_news_order_by_pubDate() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid firstId = RedBullPlayer::Tools::UuidCreator::create();
    INews* firstNews = new News( firstId, "FirstNews" );
    firstNews->setAuthor( "FirstAuthor" );
    firstNews->setCategory( "FirstCategory" );
    firstNews->setContent( "FirstContent" );
    firstNews->setExternalGuid( "FirstExternalGuid" );
    firstNews->setLink( "FirstLink" );
    firstNews->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *firstNews, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid secondId = RedBullPlayer::Tools::UuidCreator::create();
    INews* secondNews = new News( secondId, "SecondNews" );
    secondNews->setAuthor( "SecondAuthor" );
    secondNews->setCategory( "SecondCategory" );
    secondNews->setContent( "SecondContent" );
    secondNews->setExternalGuid( "SecondExternalGuid" );
    secondNews->setLink( "SecondLink" );
    secondNews->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *secondNews, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QList<INews*>* allNews = rep->allNews( true );
    QVERIFY( allNews->count() == 2 );
    QVERIFY( allNews->at( 0 )->guid() == firstId );
    QVERIFY( allNews->at( 1 )->guid() == secondId );
}

void NewsRepositoryTest::should_have_news_image() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid newsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = new NewsImage( newsId, newsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    newsImage->setHeight( img.height() );
    newsImage->setWidth( img.width() );
    newsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    newsImage->setImage( img );
    bool success = rep->saveNewsImage( *newsImage, &err );

    if ( err.errorType() != Error::NoError ) {
        qDebug() << err.errorMessage();
    }

    QVERIFY( success );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QVERIFY( db->newsRepository()->anyNewsImageExists( news->guid() ) );
}

void NewsRepositoryTest::should_save_news_image() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid newsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = new NewsImage( newsId, newsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    newsImage->setHeight( img.height() );
    newsImage->setWidth( img.width() );
    newsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    newsImage->setImage( img );
    bool success = rep->saveNewsImage( *newsImage, &err );

    if ( err.errorType() != Error::NoError ) {
        qDebug() << err.errorMessage();
    }

    QVERIFY( success );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
}

void NewsRepositoryTest::should_get_news_image_by_fileName() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid newsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = new NewsImage( newsId, newsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    newsImage->setHeight( img.height() );
    newsImage->setWidth( img.width() );
    newsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    newsImage->setImage( img );
    bool success = rep->saveNewsImage( *newsImage, &err );

    if ( err.errorType() != Error::NoError ) {
        qDebug() << err.errorMessage();
    }

    QVERIFY( success );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    INewsImage* gotImage = rep->getNewsImageByFileName( newsId, "RedBullLogo.png" );
    QVERIFY( gotImage != 0 );
    QVERIFY( !gotImage->image().isNull() );
    QVERIFY( gotImage->fileName() == "RedBullLogo.png" );
    QVERIFY( gotImage->guid() == newsImageId );
    QVERIFY( gotImage->newsGuid() == newsId );
    QVERIFY( gotImage->height() == img.height() );
    QVERIFY( gotImage->width() == img.width() );
    QVERIFY( gotImage->size() == QFileInfo( "RedBullLogo.png" ).size() );
}

void NewsRepositoryTest::should_update_news_image() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid newsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = new NewsImage( newsId, newsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    newsImage->setHeight( img.height() );
    newsImage->setWidth( img.width() );
    newsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    newsImage->setImage( img );
    bool success = rep->saveNewsImage( *newsImage, &err );

    if ( !success )
        qDebug() << err.errorMessage();

    QVERIFY( success );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    newsImage->setFileName( "NewFileName" );
    success = rep->saveNewsImage( *newsImage, &err );

    if ( !success )
        qDebug() << err.errorMessage();

    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    INewsImage* gotImage = rep->getNewsImageByFileName( newsId, "NewFileName" );
    QVERIFY( gotImage != 0 );
    QVERIFY( gotImage->fileName() == "NewFileName" );
}

void NewsRepositoryTest::should_delete_news_image() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid newsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* newsImage = new NewsImage( newsId, newsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    newsImage->setHeight( img.height() );
    newsImage->setWidth( img.width() );
    newsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    newsImage->setImage( img );
    QVERIFY( rep->saveNewsImage( *newsImage, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QVERIFY( rep->deleteNewsImage( *newsImage, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    INewsImage* gotImage = rep->getNewsImageByFileName( newsId, "RedBullLogo.png" );
    QVERIFY( gotImage == 0 );
}

void NewsRepositoryTest::should_get_all_news_images() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    INewsRepository* rep = db->newsRepository();
    QUuid newsId = RedBullPlayer::Tools::UuidCreator::create();
    INews* news = new News( newsId, "News" );
    news->setAuthor( "Author" );
    news->setCategory( "Category" );
    news->setContent( "Content" );
    news->setExternalGuid( "ExternalGuid" );
    news->setLink( "Link" );
    news->setPubDate( QDateTime::currentDateTime() );
    QVERIFY( rep->saveNews( *news, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid firstNewsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* firstNewsImage = new NewsImage( newsId, firstNewsImageId, "RedBullLogo.png" );
    QImage img( "RedBullLogo.png" );
    QVERIFY( !img.isNull() );
    firstNewsImage->setHeight( img.height() );
    firstNewsImage->setWidth( img.width() );
    firstNewsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    firstNewsImage->setImage( img );
    QVERIFY( rep->saveNewsImage( *firstNewsImage, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid secondNewsImageId = RedBullPlayer::Tools::UuidCreator::create();
    INewsImage* secondNewsImage = new NewsImage( newsId, secondNewsImageId, "RedBullLogo.png" );
    secondNewsImage->setHeight( img.height() );
    secondNewsImage->setWidth( img.width() );
    secondNewsImage->setSize( QFileInfo( "RedBullLogo.png" ).size() );
    secondNewsImage->setImage( img );
    QVERIFY( rep->saveNewsImage( *secondNewsImage, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QList<INewsImage*>* allNewsImages = rep->allNewsImages( newsId );
    QVERIFY( allNewsImages->count() == 2 );
}

