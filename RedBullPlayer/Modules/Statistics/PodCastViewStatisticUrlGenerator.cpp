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
#include "PodCastViewStatisticUrlGenerator.h"

#include <QDebug>

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../../ToolLib/helper.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Updater;

QUrl PodCastViewStatisticUrlGenerator::createRequestUrl( const RedBullPlayer::Modules::MediaLibrary::IStatisticEntry& entry ) {
    QString date = QString( "%1" ).arg( entry.date().toUTC().toTime_t() );
    QString urlTemplate = Constants::STATISTIC_URL_TEMPLATE;
    QString serverUrl = ConfigManager::instance()->getStatisticServerUrl();

    if ( serverUrl.endsWith( "/" ) ) {
        serverUrl.chop( 1 );
    }

    QString encodedData = QString( entry.data().toUtf8().toHex() );
    urlTemplate = urlTemplate.replace( Constants::STATISTIC_SERVER_URL_KEY, serverUrl );
    urlTemplate = urlTemplate.replace( Constants::STATISTIC_DATE_URL_KEY, date );
    urlTemplate = urlTemplate.replace( Constants::STATISTIC_API_KEY_KEY, _apiKey );
    urlTemplate = urlTemplate.replace( Constants::STATISTIC_TYPE_URL_KEY, Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW_URL_VALUE );
    urlTemplate = urlTemplate.replace( Constants::STATISTIC_DATA_URL_KEY, encodedData );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_OS_URL_KEY, QString( Helper::getOsVersion().toUtf8().toHex() ) );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_VERSION_URL_KEY, QString( PlayerVersion::instance()->getVersionString().toUtf8().toHex() ) );
    QUrl u( urlTemplate, QUrl::TolerantMode );
    return u;
}

void PodCastViewStatisticUrlGenerator::setPlayerUuid( QUuid uuid ) {
    _playerUuid = uuid;
}

void PodCastViewStatisticUrlGenerator::setApiKey( QString apiKey ) {
    _apiKey = apiKey;
}
