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
#include "UrlGeneratorFactoryTest.h"

// QT plugins
#include <QCoreApplication>
#include <QSignalSpy>


// Project events
#include "Constants.h"
#include "Error.h"
#include "MediaDB.h"
#include "StatisticsModule.h"
#include "EventTypes.h"
#include "InternetConnectionStatusChangedEvent.h"
#include "Statistics/CreateStatisticEntryEvent.h"

#include "StatisticClientCacheRepostoryFake.h"

#include "../../ToolLib/UuidCreator.h"
#include "../../Modules/MediaLibrary/StatisticEntry.h"
#include "../../Modules/Statistics/UrlGeneratorFactory.h"
#include "../../Modules/Statistics/PlayerStartedUrlGenerator.h"
#include "../../Modules/Statistics/PodCastViewStatisticUrlGenerator.h"
#include "../../Interfaces/Statistics/IUrlGenerator.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

void UrlGeneratorFactoryTest:: should_return_0_on_unknown_type() {
    StatisticEntry* e = new StatisticEntry( UuidCreator::create(), QString::null, QString::null, QDateTime::currentDateTime() );
    QVERIFY2( UrlGeneratorFactory::getGeratorForRequest( *e ) == 0, "Must return 0 pointer" );
    e = new StatisticEntry( UuidCreator::create(), "", QString::null, QDateTime::currentDateTime() );
    QVERIFY2( UrlGeneratorFactory::getGeratorForRequest( *e ) == 0, "Must return 0 pointer" );
    e = new StatisticEntry( UuidCreator::create(), "bla", QString::null, QDateTime::currentDateTime() );
    QVERIFY2( UrlGeneratorFactory::getGeratorForRequest( *e ) == 0, "Must return 0 pointer" );
}


void UrlGeneratorFactoryTest::should_return_player_started_url_generator() {
    StatisticEntry* e = new StatisticEntry( UuidCreator::create(), Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, QString::null, QDateTime::currentDateTime() );
    IUrlGenerator* g = UrlGeneratorFactory::getGeratorForRequest( *e );
    QVERIFY2( g != 0, "Generator must not be 0" );
    QVERIFY2( dynamic_cast<PlayerStartedUrlGenerator*>( g ) != 0, "Generator must be of type PlayerStartedUrlGenerator" );
}

void UrlGeneratorFactoryTest::should_return_pod_cast_view_url_generator() {
    StatisticEntry* e = new StatisticEntry( UuidCreator::create(), Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW, UuidCreator::create().toString(), QDateTime::currentDateTime() );
    IUrlGenerator* g = UrlGeneratorFactory::getGeratorForRequest( *e );
    QVERIFY2( g != 0, "Generator must not be 0" );
    QVERIFY2( dynamic_cast<PodCastViewStatisticUrlGenerator*>( g ) != 0, "Generator must be of type PodCastViewStatisticUrlGenerator" );
}
