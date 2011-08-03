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
#include "ConfigFileTaskTest.h"

// Qt Includes
#include <QFileInfo>
#include <QString>


// Project Includes
#include "Package.h"
#include "UpdateTask.h"
#include "ConfigFileTask.h"

using namespace RedBullPlayer::Updater;

//void ConfigFileTaskTest::should_extract_config_ini_to_temp_folder_as_config_ini_new() {
//    Package* p = Package::openCreate( QFileInfo( "data1.dat" ) );
//    p->addFile( "data2.dat", "extractPath/data2.dat" );
//    p->addFile( "data3.dat", "extractPath/data3.dat" );
//    p->addFile( "config.ini", "config.ini" );
//
//    p->save( "testPackage" );
//    p->deleteLater();
//
//
//    ConfigFileTask* task = new ConfigFileTask( "name","testPackage" );
//    task->execute();
//
//
//    QVERIFY2( QFileInfo("tmp/config.ini.new").exists(), "Should have extracted config.ini" );
//    delete task;
//}
