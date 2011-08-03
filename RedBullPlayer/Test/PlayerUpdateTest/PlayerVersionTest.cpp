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
#include "PlayerVersionTest.h"

#include "PlayerVersion.h"

using namespace RedBullPlayer::Updater;

void PlayerVersionTest::should_create_player_version() {
    PlayerVersion* playerVersion = new PlayerVersion( this, "testVersion.nfo" );
    QVERIFY( playerVersion->getMajorVersion() == "1" );
    QVERIFY( playerVersion->getMinorVersion() == "2" );
    QVERIFY( playerVersion->getBuild() == "3" );
    QVERIFY( playerVersion->getTeamCityBuildNumber() == "4" );
    QVERIFY( playerVersion->getVersionString() == "1.2.3.4" );
}
