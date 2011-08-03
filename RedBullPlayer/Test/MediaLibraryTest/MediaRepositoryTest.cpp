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
#include "MediaRepositoryTest.h"

#include <QString>
#include <QDebug>
#include <QUuid>
#include <QImage>

#include "MediaLibrary/IMediaRepository.h"
#include "MediaLibrary/IMedia.h"
#include "MediaLibrary/ITagRepository.h"
#include "MediaLibrary/ITag.h"
#include "MediaLibrary/IMediaDB.h"
#include "Shell.h"
#include "Error.h"
#include "TagRepository.h"
#include "MediaRepository.h"
#include "Media.h"
#include "Tag.h"
#include "MediaDB.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;

MediaRepositoryTest::MediaRepositoryTest() {
}


void MediaRepositoryTest::saveMedia_GetByGuid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    IMediaType* mediaType = db->mediaTypes().at( 0 );
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media media( id,
                 "fileName",
                 *mediaType,
                 1,
                 2,
                 "title",
                 "artist",
                 "genre",
                 "copyright",
                 "album",
                 "description",
                 "rating",
                 date,
                 "setting",
                 "url",
                 "language",
                 "publisher",
                 "encodedBy",
                 "artworkUrl",
                 "trackID",
                 true,
                 5,
                 1
               );
    QVERIFY( rep->saveMedia( media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    IMedia* medRet = rep->getMediaByGuid( id );
    QVERIFY( medRet != 0 );
    QVERIFY( medRet->guid() == id );
    QVERIFY( medRet->fileName() == "fileName" );
    QVERIFY( medRet->mediaType().guid() == mediaType->guid() );
    QVERIFY( medRet->mediaType().name() == mediaType->name() );
    QVERIFY( medRet->length() == 1 );
    QVERIFY( medRet->trackNumber() == 2 );
    QVERIFY( medRet->title() == "title" );
    QVERIFY( medRet->artist() == "artist" );
    QVERIFY( medRet->genre() == "genre" );
    QVERIFY( medRet->copyright() == "copyright" );
    QVERIFY( medRet->album() == "album" );
    QVERIFY( medRet->description() == "description" );
    QVERIFY( medRet->rating() == "rating" );
    QVERIFY( medRet->date().toTime_t () == date.toTime_t() );
    QVERIFY( medRet->setting() == "setting" );
    QVERIFY( medRet->url() == "url" );
    QVERIFY( medRet->language() == "language" );
    QVERIFY( medRet->publisher() == "publisher" );
    QVERIFY( medRet->encodedBy() == "encodedBy" );
    QVERIFY( medRet->artworkUrl() == "artworkUrl" );
    QVERIFY( medRet->trackID() == "trackID" );
    QVERIFY( medRet->coverImage().isNull() );
    QVERIFY( medRet->isNew() );
    QVERIFY( medRet->fileSize() == 5 );
    QVERIFY( medRet->startTime() == 1 );
    QImage img( "RedBullLogo.png" );
    medRet->setAlbum( "album2" );
    medRet->setTitle( "title2" );
    medRet->setArtist( "artist2" );
    medRet->setArtworkUrl( "artworkUrl2" );
    medRet->setCopyright( "copyright2" );
    medRet->setDescription( "description2" );
    medRet->setEncodedBy( "encodedBy2" );
    medRet->setGenre( "genre2" );
    medRet->setLanguage( "language2" );
    medRet->setLength( 3 );
    medRet->setPublisher( "publisher2" );
    medRet->setRating( "rating2" );
    medRet->setSetting( "setting2" );
    medRet->setTrackID( "trackID2" );
    medRet->setTrackNumber( 4 );
    medRet->setUrl( "url2" );
    medRet->setIsNew( false );
    medRet->setFileSize( 10 );
    medRet->setStartTime( 5 );
    date = date.addYears( 10 );
    medRet->setDate( date );
    medRet->setCoverImage( img );
    medRet->setStartAnimationFile( "SomeFile" );
    medRet->setAnimationFrameCount( 10 );
    QVERIFY( rep->saveMedia( *medRet, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    IMedia* medRet2 = rep->getMediaByGuid( id );
    QVERIFY( medRet2 != 0 );
    QVERIFY( medRet2->guid() == id );
    QVERIFY( medRet2->fileName() == "fileName" );
    QVERIFY( medRet2->mediaType().guid() == mediaType->guid() );
    QVERIFY( medRet2->mediaType().name() == mediaType->name() );
    QVERIFY( medRet2->length() == 3 );
    QVERIFY( medRet2->trackNumber() == 4 );
    QVERIFY( medRet2->title() == "title2" );
    QVERIFY( medRet2->artist() == "artist2" );
    QVERIFY( medRet2->genre() == "genre2" );
    QVERIFY( medRet2->copyright() == "copyright2" );
    QVERIFY( medRet2->album() == "album2" );
    QVERIFY( medRet2->description() == "description2" );
    QVERIFY( medRet2->rating() == "rating2" );
    QVERIFY( medRet2->date().toTime_t () == date.toTime_t() );
    QVERIFY( medRet2->setting() == "setting2" );
    QVERIFY( medRet2->url() == "url2" );
    QVERIFY( medRet2->language() == "language2" );
    QVERIFY( medRet2->publisher() == "publisher2" );
    QVERIFY( medRet2->encodedBy() == "encodedBy2" );
    QVERIFY( medRet2->artworkUrl() == "artworkUrl2" );
    QVERIFY( medRet2->trackID() == "trackID2" );
    QVERIFY( ! medRet2->coverImage().isNull() );
    QVERIFY( ! medRet2->isNew() );
    QVERIFY( medRet2->fileSize() == 10 );
    QVERIFY( medRet2->startTime() == 5 );
    QVERIFY( medRet2->startAnimationFile() == "SomeFile" );
    QVERIFY( medRet2->animationFrameCount() == 10 );
    QString newImageFileName = RedBullPlayer::Tools::UuidCreator::create() + "_loaded_from_db.png";
    medRet2->coverImage().save( newImageFileName );
    QVERIFY( QFile::exists( newImageFileName ) );
    QFile::remove( newImageFileName );
}

void MediaRepositoryTest::deleteMedia() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    ITagRepository* tagRep = db->tagRepository();
    IMediaType* mediaType = db->mediaTypes().at( 0 );
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media media( id, "fileName", *mediaType,  1,  2, "title", "artist", "genre", "copyright", "album",
                 "description", "rating", date, "setting", "url", "language", "publisher",
                 "encodedBy", "artworkUrl", "trackID" );
    QUuid idTag1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag1 = new Tag( idTag1, "idTag1", false );
    QVERIFY( tagRep->saveTag( *tag1, &err ) );
    QUuid idTag2 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag2 = new Tag( idTag2, "idTag2", false );
    QVERIFY( tagRep->saveTag( *tag2, &err ) );
    QVERIFY( rep->addTag( media, *tag1, &err ) );
    QVERIFY( rep->addTag( media, *tag2, &err ) );
    QVERIFY( rep->saveMedia( media, &err ) );
    QVERIFY( rep->allMedia()->count() == 1 );
    QVERIFY( rep->deleteMedia( media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->allMedia()->count() == 0 );
}

void MediaRepositoryTest::getAllMedia() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    IMediaType* mediaType = db->mediaTypes().at( 0 );
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media media( id, "fileName", *mediaType,  1,  2, "title", "artist", "genre", "copyright", "album",
                 "description", "rating", date, "setting", "url", "language", "publisher",
                 "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( media, &err ) );
    QUuid id2 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date2 = QDateTime::currentDateTime();
    Media media2( id2, "fileName", *mediaType,  1,  2, "title", "artist", "genre", "copyright", "album",
                  "description", "rating", date2, "setting", "url", "language", "publisher",
                  "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( media2, &err ) );
    QUuid id3 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date3 = QDateTime::currentDateTime();
    Media media3( id3, "fileName", *mediaType,  1,  2, "title", "artist", "genre", "copyright", "album",
                  "description", "rating", date3, "setting", "url", "language", "publisher",
                  "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( media3, &err ) );
    QList<IMedia*> all = *rep->allMedia();
    bool m1 = false;
    bool m2 = false;
    bool m3 = false;

    for( int i = 0; i < all.count(); i++ ) {
        if ( all.at( i )->guid() == id ) m1 = true;

        if ( all.at( i )->guid() == id2 ) m2 = true;

        if ( all.at( i )->guid() == id3 ) m3 = true;
    }

    QVERIFY( all.count() == 3 );
    QVERIFY( m1 );
    QVERIFY( m2 );
    QVERIFY( m3 );
}

void MediaRepositoryTest::getMediaBy_FileName() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    IMediaType* mediaType = db->mediaTypes().at( 0 );
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media media( id, "fileName", *mediaType,  1,  2, "title", "artist", "genre", "copyright", "album",
                 "description", "rating", date, "setting", "url", "language", "publisher",
                 "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( media, &err ) );
    IMedia* medRet = rep->getMediaByFileName( "fileName" );
    QVERIFY( medRet != 0 );
    QVERIFY( medRet->guid() == id );
    QVERIFY( medRet->fileName() == "fileName" );
}

void MediaRepositoryTest::getMediaBy_MediaType() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    IMediaType* videoType = db->videoType();
    IMediaType* audioType = db->audioType();
    IMediaType* imageType = db->imageType();
    QVERIFY( videoType != 0 );
    QVERIFY( audioType != 0 );
    QVERIFY( imageType != 0 );
    QVERIFY( rep != 0 );
    QUuid idVideo1 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media mediaVideo1( idVideo1, "idVideo1", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo2( idVideo2, "idVideo2", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo3( idVideo3, "idVideo3", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio1( idAudio1, "idAudio1", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio2( idAudio2, "idAudio2", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio3( idAudio3, "idAudio3", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage1( idImage1, "idImage1", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage2( idImage2, "idImage2", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage3( idImage3, "idImage3", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QList<IMedia*> videos = *rep->getMediaByMediaType( *videoType );
    bool v1 = false;
    bool v2 = false;
    bool v3 = false;

    for( int i = 0; i < videos.count(); i++ ) {
        if ( videos.at( i )->guid() == idVideo1 ) v1 = true;

        if ( videos.at( i )->guid() == idVideo2 ) v2 = true;

        if ( videos.at( i )->guid() == idVideo3 ) v3 = true;
    }

    QVERIFY( videos.count() == 3 );
    QVERIFY( v1 );
    QVERIFY( v2 );
    QVERIFY( v3 );
    QList<IMedia*> audios = *rep->getMediaByMediaType( *audioType );
    bool a1 = false;
    bool a2 = false;
    bool a3 = false;

    for( int i = 0; i < audios.count(); i++ ) {
        if ( audios.at( i )->guid() == idAudio1 ) a1 = true;

        if ( audios.at( i )->guid() == idAudio2 ) a2 = true;

        if ( audios.at( i )->guid() == idAudio3 ) a3 = true;
    }

    QVERIFY( audios.count() == 3 );
    QVERIFY( a1 );
    QVERIFY( a2 );
    QVERIFY( a3 );
    QList<IMedia*> images = *rep->getMediaByMediaType( *imageType );
    bool i1 = false;
    bool i2 = false;
    bool i3 = false;

    for( int i = 0; i < images.count(); i++ ) {
        if ( images.at( i )->guid() == idImage1 ) i1 = true;

        if ( images.at( i )->guid() == idImage2 ) i2 = true;

        if ( images.at( i )->guid() == idImage3 ) i3 = true;
    }

    QVERIFY( images.count() == 3 );
    QVERIFY( i1 );
    QVERIFY( i2 );
    QVERIFY( i3 );
}


void MediaRepositoryTest::addTag_getByTag() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    QVERIFY( rep != 0 );
    ITagRepository* tagRep = db->tagRepository();
    QVERIFY( tagRep != 0 );
    IMediaType* videoType = db->videoType();
    IMediaType* audioType = db->audioType();
    IMediaType* imageType = db->imageType();
    QUuid idTag1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag1 = new Tag( idTag1, "idTag1", false );
    QVERIFY( tagRep->saveTag( *tag1, &err ) );
    QUuid idTag2 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag2 = new Tag( idTag2, "idTag2", false );
    QVERIFY( tagRep->saveTag( *tag2, &err ) );
    QUuid idTag3 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag3 = new Tag( idTag3, "idTag3", false );
    QVERIFY( tagRep->saveTag( *tag3, &err ) );
    QUuid idVideo1 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media mediaVideo1( idVideo1, "idVideo1", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo2( idVideo2, "idVideo2", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo3( idVideo3, "idVideo3", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio1( idAudio1, "idAudio1", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio2( idAudio2, "idAudio2", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio3( idAudio3, "idAudio3", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaAudio3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage1( idImage1, "idImage1", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage2( idImage2, "idImage2", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage3( idImage3, "idImage3", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaImage3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaAudio1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaAudio2, *tag2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( ! rep->addTag( mediaVideo1, *tag3, &err ) );
    QVERIFY( err.errorType() == Error::Normal );
    QList<IMedia*> tag1ret = *rep->getMediaByTag( *tag1 );
    bool v1 = false;
    bool a1 = false;

    for( int i = 0; i < tag1ret.count(); i++ ) {
        if ( tag1ret.at( i )->guid() == idVideo1 ) v1 = true;

        if ( tag1ret.at( i )->guid() == idAudio1 ) a1 = true;
    }

    QVERIFY( tag1ret.count() == 2 );
    QVERIFY( v1 );
    QVERIFY( a1 );
    QList<IMedia*> tag2ret = *rep->getMediaByTag( *tag2 );
    v1 = false;
    bool a2 = false;

    for( int i = 0; i < tag2ret.count(); i++ ) {
        if ( tag2ret.at( i )->guid() == idVideo1 ) v1 = true;

        if ( tag2ret.at( i )->guid() == idAudio2 ) a2 = true;
    }

    QVERIFY( tag2ret.count() == 2 );
    QVERIFY( v1 );
    QVERIFY( a2 );
    QList<IMedia*> tag3ret = *rep->getMediaByTag( *tag3 );
    v1 = false;

    for( int i = 0; i < tag3ret.count(); i++ ) {
        if ( tag3ret.at( i )->guid() == idVideo1 ) v1 = true;
    }

    QVERIFY( tag3ret.count() == 1 );
    QVERIFY( v1 );
}


void MediaRepositoryTest::removeTag_getByTag() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    QVERIFY( rep != 0 );
    ITagRepository* tagRep = db->tagRepository();
    QVERIFY( tagRep != 0 );
    IMediaType* videoType = db->videoType();
    QUuid idTag1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag1 = new Tag( idTag1, "idTag1", false );
    QVERIFY( tagRep->saveTag( *tag1, &err ) );
    QUuid idVideo1 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media mediaVideo1( idVideo1, "idVideo1", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QList<IMedia*> tag1ret = *rep->getMediaByTag( *tag1 );
    bool v1 = false;

    for( int i = 0; i < tag1ret.count(); i++ ) {
        if ( tag1ret.at( i )->guid() == idVideo1 ) v1 = true;
    }

    QVERIFY( tag1ret.count() == 1 );
    QVERIFY( v1 );
    QVERIFY( rep->removeTag( mediaVideo1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    tag1ret = *rep->getMediaByTag( *tag1 );
    QVERIFY( tag1ret.count() == 0 );
}

