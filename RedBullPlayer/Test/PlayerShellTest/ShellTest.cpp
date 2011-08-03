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
#include "ShellTest.h"

#include "Shell.h"

using namespace RedBullPlayer::PlayerShell;

void ShellTest::registerRegion() {
    Shell* s = new Shell( 0 );
    QWidget* w = new QWidget( 0 );
    RedBullPlayer::PlayerShell::Region* r = new RedBullPlayer::PlayerShell::Region( this, "r", w );
    s->registerRegion( r );
    QVERIFY( s->regions().contains( r ) );
}


void ShellTest::initMainRegionsSetup() {
    Shell* s = new Shell( 0 );
    RedBullPlayer::PlayerShell::Region* root = s->findRegion( "Root" );
    QVERIFY( root != 0 );
}

void ShellTest::findRegion() {
    Shell* s = new Shell( 0 );
    QVERIFY( s->findRegion( "asd" ) == 0 );
    QVERIFY( s->findRegion( "Root" ) != 0 );
    QVERIFY( s->findRegion( "Root" ) == s->RootRegion() );
}
