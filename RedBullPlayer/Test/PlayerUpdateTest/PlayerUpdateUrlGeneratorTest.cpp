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
#include "PlayerUpdateUrlGeneratorTest.h"

// QT plugins
#include <QCoreApplication>
#include <QSignalSpy>

// Project events
#include "Constants.h"

#include "../../ToolLib/UuidCreator.h"
#include "../../Modules/PlayerUpdateChecker/UpdateServiceUrlGenerator.h"
#include "../../ToolLib/helper.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::PlayerUpdateChecker;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Updater;

// FIXME: Reimplement correctly
//void PlayerUpdateUrlGeneratorTest::should_create_correct_url() {
//
//    QUuid playerUuid = UuidCreator::create();
//    PlayerVersion* playerVersion = new PlayerVersion(this, "testVersion.nfo");
//
//    UpdateServiceUrlGenerator g(playerUuid, "string", playerVersion, 0);
//
//    QString serverUrl = ConfigManager::instance()->getUpdateServerUrl();
//    if (serverUrl.endsWith("/")) {
//       serverUrl.chop(1);
//    }
//
//    QString urlTemplate = Constants::UPDATE_URL_TEMPLATE;
//    urlTemplate = urlTemplate.replace(Constants::UPDATE_SERVER_URL_KEY,serverUrl);
//    urlTemplate = urlTemplate.replace(Constants::UPDATE_API_KEY_KEY,"string");
//    urlTemplate = urlTemplate.replace(Constants::UPDATE_ID_URL_KEY,QString(playerUuid.toString().toUtf8().toHex()));
//    urlTemplate = urlTemplate.replace(Constants::UPDATE_OS_URL_KEY,QString(Helper::getOsVersion().toUtf8().toHex()));
//    urlTemplate = urlTemplate.replace(Constants::UPDATE_VERSION_URL_KEY,QString(playerVersion->getVersionString().toUtf8().toHex()));
//
//    QUrl u = g.generateUrl();
//
//    QVERIFY2( ! u.isEmpty() , "Url must not be empty");
//    QVERIFY2(u.toString().compare(urlTemplate) == 0, QString("Generated Url (%1) is not like the templated Url (%2).").arg(u.toString()).arg(urlTemplate).toLatin1());
//}
