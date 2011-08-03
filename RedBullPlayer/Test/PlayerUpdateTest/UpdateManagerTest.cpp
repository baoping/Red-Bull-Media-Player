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
#include "UpdateManagerTest.h"


using namespace RedBullPlayer::Updater;

void UpdateManagerTest::should_initialize_with_empty_process_list() {
    UpdateManager *m = new UpdateManager( this );
    QVERIFY2( m->prcesses().isEmpty(), "Should initialize with emty process list" );
}

void UpdateManagerTest::should_add_process_and_sort_correctly() {
    UpdateManager *m = new UpdateManager( this );
    UpdateProcess *p1 = new UpdateProcess( "1.0.0", "1.0.1", m );
    UpdateProcess *p2 = new UpdateProcess( "1.0.1", "1.0.2", m );
    UpdateProcess *p3 = new UpdateProcess( "1.0.2", "1.0.3", m );
    m->addProcess( p3 );
    m->addProcess( p1 );
    m->addProcess( p2 );
    QVERIFY2( m->prcesses().at( 0 ) == p1, "Should have p1 as first" );
    QVERIFY2( m->prcesses().at( 1 ) == p2, "Should have p2 as second" );
    QVERIFY2( m->prcesses().at( 2 ) == p3, "Should have p3 as third" );
}

void UpdateManagerTest::should_resolve_required_processes() {
    UpdateManager *m = new UpdateManager( this );
    UpdateProcess *p1 = new UpdateProcess( "1.0.0", "1.0.1", m );
    UpdateProcess *p2 = new UpdateProcess( "1.0.1", "1.0.2", m );
    UpdateProcess *p3 = new UpdateProcess( "1.0.1", "1.0.3", m );
    m->addProcess( p1 );
    m->addProcess( p2 );
    m->addProcess( p3 );
    UpdateProcess* resolvedProcess = m->findMatchingProcess(
                                         new PlayerVersion( "1.0.1", this ),
                                         new PlayerVersion( "1.0.3", this ) );
    QVERIFY2( resolvedProcess == p3, "Should have p2 as first" );
    resolvedProcess = m->findMatchingProcess(
                          new PlayerVersion( "1.0.1", this ),
                          new PlayerVersion( "1.0.2", this ) );
    QVERIFY2( resolvedProcess == p2, "Should have p2 as first" );
    resolvedProcess = m->findMatchingProcess(
                          new PlayerVersion( "1.0.0", this ),
                          new PlayerVersion( "1.0.3", this ) );
    QVERIFY2( resolvedProcess == 0, "Should have no matching rpocess" );
}

