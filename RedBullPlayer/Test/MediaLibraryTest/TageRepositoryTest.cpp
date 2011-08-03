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
#include "TageRepositoryTest.h"

#include <QString>
#include <QDebug>
#include <QUuid>

#include "MediaLibrary/ITagRepository.h"
#include "MediaLibrary/IMediaRepository.h"
#include "MediaLibrary/ITag.h"
#include "MediaLibrary/IMediaDB.h"
#include "Shell.h"
#include "Error.h"
#include "TagRepository.h"
#include "MediaRepository.h"
#include "Tag.h"
#include "Media.h"
#include "MediaDB.h"
#include "UuidCreator.h"
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;

TageRepositoryTest::TageRepositoryTest() {
}

void TageRepositoryTest::saveTagUpdate() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ITagRepository* rep  = db->tagRepository();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    Tag t( id, "TestTag", true );
    QVERIFY( rep->saveTag( t, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    ITag* ret = rep->getTagByGuid( id );
    QVERIFY( ret != 0 );
    QVERIFY( ret->guid() == id );
    QVERIFY( ret->name() == "TestTag" );
    QVERIFY( ret->builtIn() == true );
    Tag up( id, "TestTagMod", false );
    QVERIFY( rep->saveTag( up, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    ITag* upRet = rep->getTagByGuid( id );
    QVERIFY( upRet != 0 );
    QVERIFY( upRet->guid() == id );
    QVERIFY( upRet->name() == "TestTagMod" );
    QVERIFY( upRet->builtIn() == false );
}

void TageRepositoryTest::deleteTag() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    QVERIFY( rep != 0 );
    ITagRepository* tagRep = db->tagRepository();
    QVERIFY( tagRep != 0 );
    IMediaType* videoType = db->videoType();
    QVERIFY ( videoType != 0 );
    QUuid idTag1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag* tag1 = new Tag( idTag1, "idTag1", false );
    QVERIFY( tagRep->saveTag( *tag1, &err ) );
    QVERIFY( tagRep->tags()->count() == 1 );
    QUuid idVideo1 = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media mediaVideo1( idVideo1, "idVdeo1", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID" );
    QVERIFY( rep->saveMedia( mediaVideo1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( tagRep->deleteTag( *tag1 , &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( tagRep->tags()->count() == 0 );
}

void TageRepositoryTest::saveTag_NoDuplicateNames() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ITagRepository* rep  = db->tagRepository();
    Error err;
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    Tag t( id, "TestTag", true );
    QVERIFY( rep->saveTag( t, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    id = RedBullPlayer::Tools::UuidCreator::create();
    Tag t2( id, "TestTag", true );
    QVERIFY( ! rep->saveTag( t2, &err ) );
    QVERIFY( err.errorType() == Error::Normal );
    QVERIFY( err.errorMessage() == tr( "Tag with name %1 already exists." ).arg( "TestTag" ) );
}

void TageRepositoryTest::addTag_GetTagByGuid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ITagRepository* rep  = db->tagRepository();
    Error err;
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    Tag t( id, "TestTag", true );
    QVERIFY( rep->saveTag( t, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    ITag* ret = rep->getTagByGuid( id );
    QVERIFY( ret != 0 );
    QVERIFY( ret->guid() == id );
    QVERIFY( ret->name() == "TestTag" );
    QVERIFY( ret->builtIn() == true );
}
void TageRepositoryTest::addTag_GetTagByName() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ITagRepository* rep  = db->tagRepository();
    Error err;
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    Tag t( id, "TestTag", true );
    QVERIFY( rep->saveTag( t, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    ITag* ret = rep->getTagByName( "TestTag" );
    QVERIFY( ret != 0 );
    QVERIFY( ret->guid() == id );
    QVERIFY( ret->name() == "TestTag" );
    QVERIFY( ret->builtIn() == true );
}


void TageRepositoryTest::addTag_GetTagByNameQuery() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ITagRepository* rep  = db->tagRepository();
    Error err;
    QUuid id1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t1( id1, "TagAAAAA", true );
    QVERIFY( rep->saveTag( t1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid id2 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t2( id2, "TagAABB", false );
    QVERIFY( rep->saveTag( t2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid id3 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t3( id3, "AAAbbTagAA", true );
    QVERIFY( rep->saveTag( t3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QList<ITag*> list = *rep->getTagsByNameQuery( "Tag" );
    QVERIFY( list.count() == 0 );
    list = *rep->getTagsByNameQuery( "Tag%" );
    QVERIFY( list.count() == 2 );
    list = *rep->getTagsByNameQuery( "Tag%BB" );
    QVERIFY( list.count() == 1 );
    list = *rep->getTagsByNameQuery( "AA%Tag%AA" );
    QVERIFY( list.count() == 1 );
}

void TageRepositoryTest::tags() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ITagRepository* rep  = db->tagRepository();
    Error err;
    QUuid id1 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t1( id1, "TestTag1", true );
    QVERIFY( rep->saveTag( t1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid id2 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t2( id2, "TestTag2", false );
    QVERIFY( rep->saveTag( t2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid id3 = RedBullPlayer::Tools::UuidCreator::create();
    Tag t3( id3, "TestTag3", true );
    QVERIFY( rep->saveTag( t3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QList<ITag*> list = *rep->tags();
    QVERIFY( list.count() == 3 );
    // bool first = false;
    //  bool second = false;
    // bool third = false;
    ITag* ret1 = 0;
    ITag* ret2 = 0;
    ITag* ret3 = 0;

    for ( int i = 0; i < list.count(); i++ ) {
        ITag* t = list.at( i );

        if ( t->guid() == id1 ) ret1 = t;

        if ( t->guid() == id2 ) ret2 = t;

        if ( t->guid() == id3 ) ret3 = t;
    }

    QVERIFY( ret1 != 0 );
    QVERIFY( ret1->guid() == id1 );
    QVERIFY( ret1->name() == "TestTag1" );
    QVERIFY( ret1->builtIn() == true );
    QVERIFY( ret2 != 0 );
    QVERIFY( ret2->guid() == id2 );
    QVERIFY( ret2->name() == "TestTag2" );
    QVERIFY( ret2->builtIn() == false );
    QVERIFY( ret3 != 0 );
    QVERIFY( ret3->guid() == id3 );
    QVERIFY( ret3->name() == "TestTag3" );
    QVERIFY( ret3->builtIn() == true );
}

void TageRepositoryTest::getMediaByTag() {
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
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaVideo1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo2( idVideo2, "idVideo2", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaVideo2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idVideo3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaVideo3( idVideo3, "idVideo3", *videoType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaVideo3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio1( idAudio1, "idAudio1", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaAudio1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio2( idAudio2, "idAudio2", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaAudio2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idAudio3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaAudio3( idAudio3, "idAudio3", *audioType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaAudio3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage1 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage1( idImage1, "idImage1", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaImage1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage2 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage2( idImage2, "idImage2", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaImage2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QUuid idImage3 = RedBullPlayer::Tools::UuidCreator::create();
    Media mediaImage3( idImage3, "idImage3", *imageType,  1,  2, "title", "artist", "genre", "copyright", "album",
                       "description", "rating", date, "setting", "url", "language", "publisher",
                       "encodedBy", "artworkUrl", "trackID", false, 1, 0 );
    QVERIFY( rep->saveMedia( mediaImage3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaVideo1, *tag3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaAudio1, *tag1, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaAudio2, *tag2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( rep->addTag( mediaAudio2, *tag3, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    QList<ITag*> tag1ret = *tagRep->getTagsByMedia( mediaVideo1 );
    bool t1 = false;
    bool t2 = false;
    bool t3 = false;

    for( int i = 0; i < tag1ret.count(); i++ ) {
        if ( tag1ret.at( i )->guid() == idTag1 ) t1 = true;

        if ( tag1ret.at( i )->guid() == idTag2 ) t2 = true;

        if ( tag1ret.at( i )->guid() == idTag3 ) t3 = true;
    }

    QVERIFY( tag1ret.count() == 3 );
    QVERIFY( t1 );
    QVERIFY( t2 );
    QVERIFY( t3 );
    QList<ITag*> tag2ret = *tagRep->getTagsByMedia( mediaAudio1 );
    t1 = false;
    t2 = false;
    t3 = false;

    for( int i = 0; i < tag2ret.count(); i++ ) {
        if ( tag2ret.at( i )->guid() == idTag1 ) t1 = true;
    }

    QVERIFY( tag2ret.count() == 1 );
    QVERIFY( t1 );
    QList<ITag*> tag3ret = *tagRep->getTagsByMedia( mediaAudio2 );
    t1 = false;
    t2 = false;
    t3 = false;

    for( int i = 0; i < tag3ret.count(); i++ ) {
        if ( tag3ret.at( i )->guid() == idTag2 ) t2 = true;

        if ( tag3ret.at( i )->guid() == idTag3 ) t3 = true;
    }

    QVERIFY( tag3ret.count() == 2 );
    QVERIFY( t2 );
    QVERIFY( t3 );
}
