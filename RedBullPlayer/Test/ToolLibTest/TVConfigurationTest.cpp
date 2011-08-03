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
#include "TVConfigurationTest.h"

#include "Configuration/TVConfiguration.h"

using namespace RedBullPlayer::Tools::Configuration::TV;

void TVConfigurationTest::should_get_tv_configuration() {
    TVConfiguration* tvConfiguration = new TVConfiguration( new QSettings( "testConfig.ini", QSettings::IniFormat, this ) );
    QVERIFY( tvConfiguration->count() == 1 );
    QVERIFY( tvConfiguration->nameAt( 0 ) == "channel1Name" );
    QVERIFY( tvConfiguration->urlAt( 0 ).toString() == "channel1Url" );
    QVERIFY( !tvConfiguration->imageAt( 0 ).isNull() );
    QVERIFY( tvConfiguration->startAnimationFileAt( 0 ) == "RedBullLogo.png" );
    QVERIFY( tvConfiguration->animationFrameCountAt( 0 ) == 20 );
    /*QVERIFY( tvConfiguration->nameAt(1) == "channel2Name" );
    QVERIFY( tvConfiguration->urlAt(1).toString() == "channel2Url" );
    QVERIFY( !tvConfiguration->imageAt(1).isNull() );
    QVERIFY( tvConfiguration->startAnimationFileAt(1) == "RedBullLogo.png" );
    QVERIFY( tvConfiguration->animationFrameCountAt(1) == 20 );*/
    /*QVERIFY( tvConfiguration->nameAt(2) == "" );
    QVERIFY( tvConfiguration->urlAt(2).toString() == "" );
    QVERIFY( tvConfiguration->imageAt(2).isNull() );
    QVERIFY( tvConfiguration->startAnimationFileAt(2) == "" );
    QVERIFY( tvConfiguration->animationFrameCountAt(2) == 0 );*/
    tvConfiguration->deleteLater();
}
