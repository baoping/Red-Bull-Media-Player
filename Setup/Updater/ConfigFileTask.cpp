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
#include "ConfigFileTask.h"

// QT includes
#include <QMessageBox>

// Project includes
#include "../ToolLib/UuidCreator.h"
#include "../Package/Package.h"
#include "../Package/ArchivIndex.h"
#include "../Package/ArchivData.h"
#include "../Package/ArchivFileReference.h"

using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Tools;

ConfigFileTask::ConfigFileTask(
#if defined(Q_OS_MAC)
    QString oldAppBudleName,
#else
    QString updateFileName,
#endif
    QString taskName,
    QString configFileName
) : UpdateTask( taskName ) {
    _configFileName = configFileName;
#if defined(Q_OS_MAC)
    _oldAppBudleName = oldAppBudleName;
#else
    _updateFileName = updateFileName;
#endif
    _tmpConfigFileName = QString( "config_ini_%1" ).arg( UuidCreator::create() );
}

bool ConfigFileTask::doWork() {
    try {
        extractConfigFile();
        updateSettings();
        repackageConfigFile();
    } catch( ... ) {
        return false;
    }

    return true;
}


#if defined(Q_OS_MAC)
void ConfigFileTask::extractConfigFile() {
    QFile::copy( _oldAppBudleName + "/Contents/MacOS/" + _configFileName, _tmpConfigFileName );
    _playerSettings = new QSettings( _tmpConfigFileName, QSettings::IniFormat, this );
}
#else
void ConfigFileTask::extractConfigFile() {
    QFile::copy( _configFileName, _tmpConfigFileName );
    _playerSettings = new QSettings( _tmpConfigFileName, QSettings::IniFormat, this );
}
#endif

#if defined(Q_OS_MAC)
void ConfigFileTask::repackageConfigFile() {
    _playerSettings->sync();
    _playerSettings->deleteLater();

    if ( QFileInfo( _configFileName ).exists() ) {
        QFile::remove( _configFileName );
    }

    QFile::copy( _tmpConfigFileName, _configFileName );
    QFile::remove( _tmpConfigFileName );
}
#else
void ConfigFileTask::repackageConfigFile() {
    _playerSettings->sync();
    _playerSettings->deleteLater();
    Package *p = Package::openEdit( _updateFileName );
    p->removeFile( _configFileName );
    p->addFile( _tmpConfigFileName, _configFileName, _configFileName );
    p->save( _updateFileName );
    delete p;
}
#endif
