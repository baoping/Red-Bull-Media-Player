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
#include "VersionTest.h"

#include "Version.h"

void VersionTest::should_be_lesser_major() {
    Version firstVersion( "1.0.0.0" );
    Version secondVersion( "2.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) < 0 );
}

void VersionTest::should_be_lesser_minor() {
    Version firstVersion( "1.0.0.0" );
    Version secondVersion( "1.1.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) < 0 );
}

void VersionTest::should_be_lesser_build() {
    Version firstVersion( "1.0.0.0" );
    Version secondVersion( "1.0.1.0" );
    QVERIFY( firstVersion.compare( secondVersion ) < 0 );
}

void VersionTest::should_be_lesser_tc() {
    Version firstVersion( "1.0.0.0" );
    Version secondVersion( "1.0.0.1" );
    QVERIFY( firstVersion.compare( secondVersion ) < 0 );
}

void VersionTest::should_be_equal() {
    Version firstVersion( "1.0.0.0" );
    Version secondVersion( "1.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) == 0 );
}

void VersionTest::should_be_greater_major() {
    Version firstVersion( "2.0.0.0" );
    Version secondVersion( "1.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) > 0 );
}

void VersionTest::should_be_greater_minor() {
    Version firstVersion( "1.1.0.0" );
    Version secondVersion( "1.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) > 0 );
}

void VersionTest::should_be_greater_build() {
    Version firstVersion( "1.0.1.0" );
    Version secondVersion( "1.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) > 0 );
}

void VersionTest::should_be_greater_tc() {
    Version firstVersion( "1.0.0.1" );
    Version secondVersion( "1.0.0.0" );
    QVERIFY( firstVersion.compare( secondVersion ) > 0 );
}

void VersionTest::should_be_fuzzy_equal_minor() {
    Version firstVersion( "1" );
    Version secondVersion( "1.1" );
    QVERIFY( firstVersion.compare( secondVersion ) == 0 );
    Version firstVersion2( "1.1" );
    Version secondVersion2( "1" );
    QVERIFY( firstVersion2.compare( secondVersion2 ) == 0 );
}
void VersionTest::should_be_fuzzy_equal_build() {
    Version firstVersion( "1.0" );
    Version secondVersion( "1.0.1" );
    QVERIFY( firstVersion.compare( secondVersion ) == 0 );
    Version firstVersion2( "1.0.1" );
    Version secondVersion2( "1.0" );
    QVERIFY( firstVersion2.compare( secondVersion2 ) == 0 );
}
void VersionTest::should_be_fuzzy_equal_tc() {
    Version firstVersion( "1.0.0" );
    Version secondVersion( "1.0.0.1" );
    QVERIFY( firstVersion.compare( secondVersion ) == 0 );
    Version firstVersion2( "1.0.0.1" );
    Version secondVersion2( "1.0.0" );
    QVERIFY( firstVersion2.compare( secondVersion2 ) == 0 );
}
