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
#include "StorageFactoryTest.h"


#include "FileSystem/IStorage.h"
#include "FileSystem/StorageFactory.h"

using namespace RedBullPlayer::Tools;


void StorageFactoryTest::should_return_storage() {
    IStorage* s = StorageFactory::defaultStorage();
    QVERIFY2( s != 0, "Must return valid IStorage pointer" );
}

void StorageFactoryTest::should_return_same_storage_when_called_multiple_times() {
    IStorage* s = StorageFactory::defaultStorage();
    QVERIFY2( s != 0, "Must return valid IStorage pointer" );
    IStorage* s2 = StorageFactory::defaultStorage();
    QVERIFY2( s == s2, "Must return identical IStorage pointer" );
}
