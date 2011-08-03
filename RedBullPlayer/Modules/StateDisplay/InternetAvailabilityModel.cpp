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
#include "InternetAvailabilityModel.h"

#include <QFile>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

InternetAvailabilityModel::InternetAvailabilityModel( QObject* parent ) : IInternetAvailabilityModel( parent ) {
    QFile onlineFile( Constants::STATUSDISPLAY_TEXT_DIR + Constants::STATUSDISPLAY_ONLINE_TXT_FILE );
    onlineFile.open( QFile::ReadOnly );
    QByteArray online = onlineFile.readAll();
    onlineFile.close();
    _onlineText = QString( online );
    QFile offlineFile( Constants::STATUSDISPLAY_TEXT_DIR + Constants::STATUSDISPLAY_OFFLINE_TXT_FILE );
    offlineFile.open( QFile::ReadOnly );
    QByteArray offline = offlineFile.readAll();
    offlineFile.close();
    _offlineText = QString( offline );
}

InternetAvailabilityModel::~InternetAvailabilityModel() {}

QString InternetAvailabilityModel::onlineText() {
    return _onlineText;
}

QString InternetAvailabilityModel::offlineText() {
    return _offlineText;
}
