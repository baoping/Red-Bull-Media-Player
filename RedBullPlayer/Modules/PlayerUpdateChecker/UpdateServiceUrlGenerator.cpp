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
#include "UpdateServiceUrlGenerator.h"

#include <QDebug>

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/helper.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::PlayerUpdateChecker;
using namespace RedBullPlayer::Updater;

UpdateServiceUrlGenerator::UpdateServiceUrlGenerator( QUuid playerId, QString apiKey,
        PlayerVersion* playerVersion,
        QObject* parent )
    : QObject( parent ) {
    Q_ASSERT( !playerId.isNull() );
    Q_ASSERT( playerVersion != 0 );
    _playerId = playerId;
    _apiKey = apiKey;
    _playerVersion = playerVersion;
}

QUrl UpdateServiceUrlGenerator::generateUrl() {
    QString urlTemplate = Constants::UPDATE_URL_TEMPLATE;
    QString serverUrl = ConfigManager::instance()->getUpdateServerUrl();

    if ( serverUrl.endsWith( "/" ) ) {
        serverUrl.chop( 1 );
    }

    urlTemplate = urlTemplate.replace( Constants::UPDATE_SERVER_URL_KEY, serverUrl );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_API_KEY_KEY, _apiKey );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_ID_URL_KEY, QString( _playerId.toString().toUtf8().toHex() ) );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_OS_URL_KEY, QString( Helper::getOsVersion().toUtf8().toHex() ) );
    urlTemplate = urlTemplate.replace( Constants::UPDATE_VERSION_URL_KEY, QString( _playerVersion->getVersionString().toUtf8().toHex() ) );
    return QUrl( urlTemplate, QUrl::TolerantMode );
}




