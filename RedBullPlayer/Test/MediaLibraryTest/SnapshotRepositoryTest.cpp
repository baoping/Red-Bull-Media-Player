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
#include "SnapshotRepositoryTest.h"

#include <QString>
#include <QDebug>
#include <QUuid>
#include <QImage>
#include <QList>
#include "MediaLibrary/ITagRepository.h"
#include "MediaLibrary/IMediaRepository.h"
#include "MediaLibrary/ISnapshotRepository.h"
#include "MediaLibrary/ITag.h"
#include "MediaLibrary/ISnapshot.h"
#include "MediaLibrary/IMediaDB.h"
#include "MediaLibrary/IMedia.h"
#include "Shell.h"
#include "Error.h"
#include "TagRepository.h"
#include "MediaRepository.h"
#include "SnapshotRepository.h"
#include "Tag.h"
#include "Snapshot.h"
#include "Media.h"
#include "MediaDB.h"
#include "UuidCreator.h"
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;



SnapshotRepositoryTest::SnapshotRepositoryTest() {
}


void SnapshotRepositoryTest::saveSanpshotGetByGuid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISnapshotRepository* rep = db->snapshotRepository();
    IMediaRepository* mediRep =  db->mediaRepository();
    IMediaType* mediaType = db->videoType();
    QVERIFY( mediaType != 0 );
    IMedia *media = new Media( RedBullPlayer::Tools::UuidCreator::create(), "fileName", *mediaType );
    QVERIFY( mediRep->saveMedia( *media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    float position = 0.1F;
    ISnapshot* s = new Snapshot( id, image, position, media );
    bool worked = rep->saveSnapshot( *s, &err );
    // Test Insert
    QVERIFY( worked );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    // Test get by Guid
    ISnapshot* sFromDB = rep->getSnapshotByGuid( id, &err );
    QVERIFY( sFromDB != 0 );
    QVERIFY( s->guid() == sFromDB->guid() );
    QVERIFY( s->media()->guid() == sFromDB ->media()->guid() );
    QVERIFY( s->image() == sFromDB->image() );
    QVERIFY( s->position() == sFromDB->position() );
    // Test Update
    QImage image2( "RedBullLogoMod.png" );
    IMedia *media2 = new Media( RedBullPlayer::Tools::UuidCreator::create(), "fileName2", *mediaType );
    QVERIFY( mediRep->saveMedia( *media2, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    ISnapshot* s2 = new Snapshot( id, image2, 0.4F, media2 );
    worked = rep->saveSnapshot( *s2, &err );
    QVERIFY( worked );
    QVERIFY( err.errorType() == Error::NoError );
    ISnapshot* sFromDB2 = rep->getSnapshotByGuid( id, &err );
    QVERIFY( sFromDB2 != 0 );
    QVERIFY( sFromDB2->guid() == id );
    QVERIFY( sFromDB2->media()->guid() == media2->guid() );
    QVERIFY( sFromDB2->image() == image2 );
    QVERIFY( sFromDB2->position() == 0.4F );
}



void SnapshotRepositoryTest::deleteSnapshot() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISnapshotRepository* rep = db->snapshotRepository();
    IMediaRepository* mediRep =  db->mediaRepository();
    IMediaType* mediaType = db->videoType();
    IMedia *media = new Media( RedBullPlayer::Tools::UuidCreator::create(), "fileName", *mediaType );
    QVERIFY( mediRep->saveMedia( *media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    float positon = 0.6F;
    ISnapshot* s = new Snapshot( id, image, positon, media );
    bool worked = rep->saveSnapshot( *s, &err );
    // Do Insert
    QVERIFY( worked );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    // Do delete
    worked = rep->deleteSnapshot( *s, &err );
    QVERIFY( worked );
    QVERIFY( err.errorType() == Error::NoError );
    ISnapshot* sFromDB = rep->getSnapshotByGuid( id, &err );
    QVERIFY( sFromDB == 0 );
}

void SnapshotRepositoryTest::getByMedia() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISnapshotRepository* rep = db->snapshotRepository();
    IMediaRepository* mediRep =  db->mediaRepository();
    IMediaType* mediaType = db->videoType();
    IMedia *media = new Media( RedBullPlayer::Tools::UuidCreator::create(), "fileName", *mediaType );
    QVERIFY( mediRep->saveMedia( *media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid id1 = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    float positon1 = 0.1F;
    ISnapshot* s = new Snapshot( id1, image, positon1, media );
    rep->saveSnapshot( *s , &err );
    QUuid id2 = RedBullPlayer::Tools::UuidCreator::create();
    float positon2 = 0.2F;
    ISnapshot* s2 = new Snapshot( id2, image, positon2, media );
    rep->saveSnapshot( *s2 , &err );
    QUuid id3 = RedBullPlayer::Tools::UuidCreator::create();
    float positon3 = 0.3F;
    ISnapshot* s3 = new Snapshot( id3, image, positon3, media );
    rep->saveSnapshot( *s3 , &err );
    QUuid id4 = RedBullPlayer::Tools::UuidCreator::create();
    float positon4 = 0.4F;
    ISnapshot* s4 = new Snapshot( id4, image, positon4, media );
    rep->saveSnapshot( *s4 , &err );
    QUuid id5 = RedBullPlayer::Tools::UuidCreator::create();
    float positon5 = 0.5F;
    ISnapshot* s5 = new Snapshot( id5, image, positon5, media );
    rep->saveSnapshot( *s5 , &err );
    err.reset();
    QList<ISnapshot*>* ret = rep->getSnapshotsByMedia( media, &err );
    QVERIFY( err.errorType() == Error::NoError );
    QVERIFY( ret->count() == 5 );
    QVERIFY( ret->at( 0 )->guid() == id1 );
    QVERIFY( ret->at( 1 )->guid() == id2 );
    QVERIFY( ret->at( 2 )->guid() == id3 );
    QVERIFY( ret->at( 3 )->guid() == id4 );
    QVERIFY( ret->at( 4 )->guid() == id5 );
    QVERIFY( ret->at( 0 )->media() == media );
    QVERIFY( ret->at( 1 )->media() == media );
    QVERIFY( ret->at( 2 )->media() == media );
    QVERIFY( ret->at( 3 )->media() == media );
    QVERIFY( ret->at( 4 )->media() == media );
}

void SnapshotRepositoryTest::getByMediaAndPosition() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISnapshotRepository* rep = db->snapshotRepository();
    IMediaRepository* mediRep =  db->mediaRepository();
    IMediaType* mediaType = db->videoType();
    IMedia *media = new Media( RedBullPlayer::Tools::UuidCreator::create(), "fileName", *mediaType );
    QVERIFY( mediRep->saveMedia( *media, &err ) );
    QVERIFY( err.errorType() == Error::NoError );
    err.reset();
    QUuid id1 = RedBullPlayer::Tools::UuidCreator::create();
    QImage image( "RedBullLogo.png" );
    float positon1 = 0.1F;
    ISnapshot* s = new Snapshot( id1, image, positon1, media );
    rep->saveSnapshot( *s , &err );
    ISnapshot* ret = rep->getSnapshotsByMediaAnPosition( media, positon1, &err );
    QVERIFY( ret != 0 );
    QVERIFY( ret->guid() == id1 );
    QVERIFY( ret->media() == media );
}
