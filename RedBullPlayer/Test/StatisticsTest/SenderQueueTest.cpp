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
#include "SenderQueueTest.h"

// QT plugins
#include <QCoreApplication>
#include <QSignalSpy>


// Project events
#include "Constants.h"
#include "Error.h"
#include "MediaDB.h"
#include "StatisticsModule.h"
#include "IUrlGenerator.h"
#include "UrlGeneratorFactory.h"
#include "EventTypes.h"
#include "InternetConnectionStatusChangedEvent.h"
#include "Statistics/CreateStatisticEntryEvent.h"


#include "StatisticClientCacheRepostoryFake.h"

#include "../../ToolLib/UuidCreator.h"
#include "../../Modules/MediaLibrary/StatisticEntry.h"
using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


void SenderQueueTest::should_queue_new_request_when_internet_available() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    s->setInternetAvailable( true );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return true" );
    QVERIFY2( s->queue().contains( entry ), "Entry must be queued" );
}

void SenderQueueTest::should_not_queue_new_request_when_internet_available() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    s->setInternetAvailable( false );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( ! s->queueEntry( entry ), "queueEntry must return false" );;
    QVERIFY2( s->queue().count() == 0, "Entry must not be queued" );
}


void SenderQueueTest::should_start_timer_in_ctor() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    QVERIFY2( s->timerActive(), "Timer must be started automaticall in ctor" );
}

void SenderQueueTest::should_initialize_netwrok_access_manager_in_ctor() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    QVERIFY2( s->networkAccessManager() , "NetworkAccessManager must be initialized in ctor" );
}

void SenderQueueTest::should_initialize_currentEntry_with_0() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    QVERIFY2( s->currentEntry() == 0 , "Current Entry must be initialized with null" );
}

void SenderQueueTest::when_the_timer_shots_should_unqueue_and_set_as_current_entry() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    s->setInternetAvailable( true );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return false" );
    QUuid id2 = UuidCreator::create();
    IStatisticEntry* entry2 = new StatisticEntry( id2, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry2 ), "queueEntry must return false" );
    QUuid id3 = UuidCreator::create();
    IStatisticEntry* entry3 = new StatisticEntry( id3, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry3 ), "queueEntry must return false" );
    s->timerShot();
    QVERIFY2( s->currentEntry() == entry, "Current entry must be set" );
    QVERIFY2( ! s->queue().contains( entry ), "Entry must have been taken from queue" );
    QVERIFY2( s->queue().contains( entry2 ), "Other Entries must still been queued" );
    QVERIFY2( s->queue().contains( entry3 ), "Other Entries must still been queued" );
}

void SenderQueueTest::when_the_timer_shots_should_not_unqueue_and_set_as_current_entry_if_there_allready_is_one_set()  {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    s->setInternetAvailable( true );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return false" );
    QUuid id2 = UuidCreator::create();
    IStatisticEntry* entry2 = new StatisticEntry( id2, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry2 ), "queueEntry must return false" );
    QUuid id3 = UuidCreator::create();
    IStatisticEntry* entry3 = new StatisticEntry( id3, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry3 ), "queueEntry must return false" );
    s->timerShot();
    s->timerShot();
    s->timerShot();
    QVERIFY2( s->currentEntry() == entry, "Current entry must be set" );
    QVERIFY2( ! s->queue().contains( entry ), "Entry must have been taken from queue" );
    QVERIFY2( s->queue().contains( entry2 ), "Other Entries must still been queued" );
    QVERIFY2( s->queue().contains( entry3 ), "Other Entries must still been queued" );
}

void SenderQueueTest::should_process_all_when_no_request_fails() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    s->setInternetAvailable( true );
    // qRegisterMetaType<RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*>("RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*");
    QSignalSpy spy( s, SIGNAL( entrySentToServer( QString ) ) );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return false" );
    QUuid id2 = UuidCreator::create();
    IStatisticEntry* entry2 = new StatisticEntry( id2, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry2 ), "queueEntry must return false" );
    QUuid id3 = UuidCreator::create();
    IStatisticEntry* entry3 = new StatisticEntry( id3, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry3 ), "queueEntry must return false" );
    s->timerShot();
    s->requestFinished( QUrl( "http://www.google.com" ), QByteArray( "<html></thml>" ), "", "" );
    s->timerShot();
    s->requestFinished( QUrl( "http://www.google.com" ), QByteArray( "<html></thml>" ), "", "" );
    s->timerShot();
    s->requestFinished( QUrl( "http://www.google.com" ), QByteArray( "<html></thml>" ), "", "" );
    QVERIFY2( s->currentEntry() == 0, "Current entry must be nuzll now" );
    QVERIFY2( ! s->queue().contains( entry ), "Entry must have been taken from queue" );
    QVERIFY2( ! s->queue().contains( entry2 ), "Entry2 must have been taken from queue" );
    QVERIFY2( ! s->queue().contains( entry3 ), "Entry3 must have been taken from queue" );
    QVERIFY2( spy.count() == 3 , "Signal must be emitted 3 times" );
}

void SenderQueueTest::should_process_all_when_some_request_fails() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    qRegisterMetaType<RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*>( "RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*" );
    QSignalSpy spy( s, SIGNAL( entrySentToServerFailed( QString ) ) );
    s->setInternetAvailable( true );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return false" );
    QUuid id2 = UuidCreator::create();
    IStatisticEntry* entry2 = new StatisticEntry( id2, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry2 ), "queueEntry must return false" );
    QUuid id3 = UuidCreator::create();
    IStatisticEntry* entry3 = new StatisticEntry( id3, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "data", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry3 ), "queueEntry must return false" );
    s->timerShot();
    s->requestFailed( QUrl( "http://www.google.com" ), 202, "" );
    s->timerShot();
    s->requestFinished( QUrl( "http://www.google.com" ), QByteArray( "<html></thml>" ), "", "" );
    s->timerShot();
    s->requestFailed( QUrl( "http://www.google.com" ), 202, "" );
    QVERIFY2( s->currentEntry() == 0, "Current entry must be nuzll now" );
    QVERIFY2( ! s->queue().contains( entry ), "Entry must have been taken from queue" );
    QVERIFY2( ! s->queue().contains( entry2 ), "Entry2 must have been taken from queue" );
    QVERIFY2( ! s->queue().contains( entry3 ), "Entry3 must have been taken from queue" );
    QVERIFY2( spy.count() == 2 , "Signal must be emitted 2 times" );
}

void SenderQueueTest::should_generate_correct_url_using_factory() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    s->setInternetAvailable( true );
    QUuid id = UuidCreator::create();
    IStatisticEntry* entry = new StatisticEntry( id, Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, "", QDateTime::currentDateTime() );
    QVERIFY2( s->queueEntry( entry ), "queueEntry must return true" );
    QUrl utest = s->generateRequestUrl( *entry );
    IUrlGenerator* ug = UrlGeneratorFactory::getGeratorForRequest( *entry );
    QUrl uCorrect =  ug->createRequestUrl( *entry );
    qDebug() <<  "uTest: " << utest.toString();
    qDebug() <<  "uCorrect: " << uCorrect.toString();
    QVERIFY2( utest == uCorrect, "Url must be created correctly" );
}

