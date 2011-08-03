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
#include "StatisticsModuleTest.h"

// QT plugins
#include <QCoreApplication>


// Project events
#include "Error.h"
#include "MediaDB.h"
#include "StatisticsModule.h"
#include "EventTypes.h"
#include "InternetConnectionStatusChangedEvent.h"
#include "Statistics/CreateStatisticEntryEvent.h"
#include "Statistics/PlayerStartedStatisticsEntryEvent.h"
#include "Statistics/PodCastViewStatisticEntryEvent.h"



#include "StatisticClientCacheRepostoryFake.h"
#include "SystemMetaDataRepositoryFake.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

void StatisticsModuleTest::should_react_on_internetconnection_changed_event() {
    StatisticsModule* m = new StatisticsModule();
    QVERIFY2( ! m->internetAvailable(), "Should initialize without internet" );
    InternetConnectionStatusChangedEvent ev( true );
    QCoreApplication::sendEvent( m, &ev );
    QVERIFY2( m->internetAvailable(), "Must set internet availability due toInternetConnectionStatusChangedEvent" );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    QVERIFY2( ! m->internetAvailable(), "Must set internet availability due toInternetConnectionStatusChangedEvent" );
}

void StatisticsModuleTest::should_set_internet_availability_for_sender_queue() {
    StatisticsModule* m = new StatisticsModule();
    QVERIFY2( ! m->internetAvailable(), "Should initialize without internet" );
    InternetConnectionStatusChangedEvent ev( true );
    QCoreApplication::sendEvent( m, &ev );
    QVERIFY2( m->senderQueue()->internetAvailable(), "Must set internet availability in sender Queue" );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    QVERIFY2( ! m->senderQueue()->internetAvailable(), "Must set internet availability in sender Queue" );
}

void StatisticsModuleTest::should_initialise_the_sender_queue() {
    StatisticsModule* m = new StatisticsModule();
    QVERIFY2( m->senderQueue() != 0, "Must initialize sender queue" );
}

void StatisticsModuleTest::shouild_initialize_with_0_repository() {
    StatisticsModule* m = new StatisticsModule();
    QVERIFY2( m->statisticClientCacheRepository() == 0, "Must be initialized with 0 repository" );
}

void StatisticsModuleTest::shouild_set_repository() {
    StatisticsModule* m = new StatisticsModule();
    IStatisticClientCacheRepository* rep  = new StatisticClientCacheRepostoryFake( this );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    QVERIFY2( rep == m->statisticClientCacheRepository(), "Must setStatisticClientCacheRepository correctly" );
    m->setSystemMetaDataRepository( systemRep );
    QVERIFY2( systemRep == m->systemMetaDataRepository(), "Must setSystemMetaDataRepository correctly" );
}

void StatisticsModuleTest::should_cache_to_repository_when_no_internet_available() {
    StatisticsModule* m = new StatisticsModule();
    StatisticClientCacheRepostoryFake* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setSystemMetaDataRepository( systemRep );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    CreateStatisticEntryEvent cse( "eventType", "data" );
    QCoreApplication::sendEvent( m, &cse );
    QVERIFY2( rep->getAllEntries()->count() == 1, "Must have cached entry" );
    QVERIFY2( rep->getAllEntries()->at( 0 )->data() == QString( "data" ), "Must store correct data" );
    QVERIFY2( rep->getAllEntries()->at( 0 )->entryType() == QString( "eventType" ), "Must store correct type" );
}

void StatisticsModuleTest::should_cache_to_repository_when_internet_available() {
    StatisticsModule* m = new StatisticsModule();
    StatisticClientCacheRepostoryFake* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setSystemMetaDataRepository( systemRep );
    InternetConnectionStatusChangedEvent ev2( true );
    QCoreApplication::sendEvent( m, &ev2 );
    CreateStatisticEntryEvent cse( "eventType", "data" );
    QCoreApplication::sendEvent( m, &cse );
    QVERIFY2( rep->getAllEntries()->count() == 1, "Must not have cached entry" );
}

void StatisticsModuleTest::should_react_to_create_statistic_entry_events() {
    StatisticsModule* m = new StatisticsModule();
    StatisticClientCacheRepostoryFake* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setSystemMetaDataRepository( systemRep );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    PlayerStartedStatisticsEntryEvent pse;
    QCoreApplication::sendEvent( m, &pse );
    PodCastViewStatisticEntryEvent pcve( "bla" );
    QCoreApplication::sendEvent( m, &pcve );
    QVERIFY2( rep->getAllEntries()->count() == 2, "Must have cached entries" );
    QVERIFY( rep->getAllEntries()->at( 0 )->entryType() == Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED );
    QVERIFY( rep->getAllEntries()->at( 1 )->entryType() == Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW );
}

void StatisticsModuleTest::should_queue_cached_requests_when_internet_becomes_available() {
    StatisticsModule* m = new StatisticsModule();
    StatisticClientCacheRepostoryFake* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setSystemMetaDataRepository( systemRep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    // We do not wnat notification at the point
    s->blockSignals( true );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    PlayerStartedStatisticsEntryEvent pse;
    QCoreApplication::sendEvent( m, &pse );
    PodCastViewStatisticEntryEvent pcve( "bla" );
    QCoreApplication::sendEvent( m, &pcve );
    QVERIFY2( s->queue().isEmpty(), "Queue must be empty" );
    QVERIFY2( rep->getAllEntries()->count() == 2, "Must have cached entries" );
    InternetConnectionStatusChangedEvent enInet( true );
    QCoreApplication::sendEvent( m, &enInet );
    QCoreApplication::processEvents();
    QVERIFY2( s->queue().count() == 2, "Cached Requests must have been enqueued" );
    QVERIFY2( rep->getAllEntries()->count() == 2, "Must still be in cache" );
}


void StatisticsModuleTest::should_remove_cached_requests_only_when_sent_successfully() {
    StatisticsModule* m = new StatisticsModule();
    StatisticClientCacheRepostoryFake* rep  = new StatisticClientCacheRepostoryFake( this );
    m->setStatisticClientCacheRepository( rep );
    ISystemMetaDataRepository* systemRep = new SystemMetaDataRepositoryFake( this );
    m->setSystemMetaDataRepository( systemRep );
    SenderQueue* s = m->senderQueue();
    // Dispable timer for test
    s->timer()->stop();
    // Prevent the NAM form informing about state chancges for tests
    s->networkAccessManager()->blockSignals( true );
    InternetConnectionStatusChangedEvent ev2( false );
    QCoreApplication::sendEvent( m, &ev2 );
    PlayerStartedStatisticsEntryEvent pse;
    QCoreApplication::sendEvent( m, &pse );
    PodCastViewStatisticEntryEvent pcve( "bla" );
    QCoreApplication::sendEvent( m, &pcve );
    QVERIFY2( s->queue().isEmpty(), "Queue must be empty" );
    QVERIFY2( rep->getAllEntries()->count() == 2, "Must have cached entries" );
    InternetConnectionStatusChangedEvent enInet( true );
    QCoreApplication::sendEvent( m, &enInet );
    QCoreApplication::processEvents();
    QVERIFY2( s->queue().count() == 2, "Cached Requests must have been enqueued" );
    QVERIFY2( rep->getAllEntries()->count() == 2, "Must still be in cache" );
    s->timerShot();
    s->requestFinished( QUrl( "http://www.google.com" ), QByteArray( "<html></thml>" ), "", "" );
    s->timerShot();
    s->requestFailed( QUrl( "http://www.google.com" ), 400, "asda" );
    QVERIFY2( s->queue().count() == 0, "Cached Requests must have been enqueued" );
    QVERIFY2( rep->getAllEntries()->count() == 1, "On request Must still be in cache" );
}
