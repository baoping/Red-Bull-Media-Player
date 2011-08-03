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
#include "Win32InstallerTask.h"

#include <QFile>
#include <QTextStream>

#ifdef Q_OS_LINUX
#include <stdlib.h>
#endif

using namespace RedBullPlayer::Updater;

Win32InstallerTask::Win32InstallerTask( QString updatePackagePath, QString originalApplicationPath )
    : UpdateTask( "Installing update package..." ) {
    _updatePackagePath = updatePackagePath;
    _originalApplicationPath = originalApplicationPath;
}


bool Win32InstallerTask::doWork() {
    if ( ! QFile::remove( _originalApplicationPath ) ) {
        return false;
    }

    if ( ! QFile::copy( _updatePackagePath, _originalApplicationPath ) ) {
        return false;
    }

#ifdef Q_OS_LINUX
    QFile newPackage( _originalApplicationPath );
    newPackage.setPermissions(
        QFile::ExeGroup |
        QFile::ExeOther |
        QFile::ExeOwner |
        QFile::ExeUser |
        QFile::ReadOwner |
        QFile::ReadGroup |
        QFile::ReadOther |
        QFile::WriteOwner |
        QFile::WriteGroup
    );
#endif
    QFile::remove( _updatePackagePath );
    QFile f( "force_reinstall" );

    if ( f.open( QFile::WriteOnly ) ) {
        QTextStream stream( &f );
        stream << "force update" << endl;
        f.flush();
        f.close();
    } else {
        return false;
    }

    return true;
}

