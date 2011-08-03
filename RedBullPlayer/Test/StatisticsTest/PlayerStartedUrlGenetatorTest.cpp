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
#include "PlayerStartedUrlGenetatorTest.h"

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
#include "../../Interfaces/Statistics/IUrlGenerator.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

//void PlayerStartedUrlGenetatorTest::should_create_correct_url() {
//    StatisticEntry* e = new StatisticEntry( UuidCreator::create(), Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, QString::null, QDateTime::currentDateTime() );
//    PlayerStartedUrlGenerator g;
//
//    QString date = QString("%1").arg( e->date().toUTC().toTime_t() );
//
//    QUuid playerUuid = UuidCreator::create();
//
//    QString serverUrl = ConfigManager::instance()->getStatisticServerUrl();
//    if (serverUrl.endsWith("/")) {
//       serverUrl.chop(1);
//    }
//
//    QString urlTemplate = Constants::STATISTIC_URL_TEMPLATE;
//
//    urlTemplate = urlTemplate.replace(Constants::STATISTIC_SERVER_URL_KEY,serverUrl);
//    urlTemplate = urlTemplate.replace(Constants::STATISTIC_DATE_URL_KEY,date);
//    urlTemplate = urlTemplate.replace(Constants::STATISTIC_TYPE_URL_KEY,Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED_URL_VALUE);
//    urlTemplate = urlTemplate.replace(Constants::STATISTIC_DATA_URL_KEY,QString(playerUuid.toString().toUtf8().toHex()));
//
//
//    g.setPlayerUuid( playerUuid );
//    QUrl u = g.createRequestUrl( *e );
//
//
//
//    QVERIFY2( ! u.isEmpty() , "Url must not be empty");
//    QVERIFY2(u.toString().compare(urlTemplate) == 0, QString("Generated Url (%1) is not like the templated Url (%2).").arg(u.toString()).arg(urlTemplate).toLatin1());
//    //QVERIFY2( u.queryItemValue( Constants::STATISTIC_PLAYERID_URL_KEY ) == plaerUuid.toString(), "Url must contain correct player id item" );
//    //QVERIFY2( u.queryItemValue( Constants::STATISTIC_TYPE_URL_KEY ) == Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED_URL_VALUE, "Url must contain player started as eventType" );
//    //QVERIFY2( u.queryItemValue( Constants::STATISTIC_DATE_URL_KEY ) == date, "Url must contain event date" );
//
//    // g.createRequestUrl( )
//
//
//
//}
