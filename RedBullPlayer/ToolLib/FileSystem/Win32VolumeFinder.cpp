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
#include <qglobal.h>

#ifdef Q_OS_WIN
#include "Win32VolumeFinder.h"

// QT Includes
#include <qt_windows.h>
#include <QDir>
#include <QDebug>

using namespace RedBullPlayer::Tools;

Win32VolumeFinder::Win32VolumeFinder() {}

QString Win32VolumeFinder::TvInfo( QString tdrive ) {
    WCHAR szVolumeName[256] ;
    WCHAR szFileSystemName[256];
    DWORD dwSerialNumber = 0;
    DWORD dwMaxFileNameLength = 256;
    DWORD dwFileSystemFlags = 0;
    bool ret = GetVolumeInformation( ( WCHAR * ) tdrive.utf16(), szVolumeName, 256, &dwSerialNumber, &dwMaxFileNameLength, &dwFileSystemFlags, szFileSystemName, 256 );

    if( !ret )return QString( "" );

    QString vName = QString::fromUtf16 ( ( const ushort * ) szVolumeName ) ;
    vName.trimmed();
    return vName;
}

QString Win32VolumeFinder::findDriveByLabel( QString label ) {
    foreach( QFileInfo drive, QDir::drives() ) {
        QString la = TvInfo( drive.absoluteFilePath() );
        qDebug() << "Drive: " << drive.absoluteFilePath() << " Label: " << la;

        if ( label.toLower() == la.toLower() ) {
            return drive.absoluteFilePath();
        }
    }
    return QString::null;
}

QString Win32VolumeFinder::getVolumeLabelForPath( QString path ) {
    QFileInfo inf( path );

    if ( ! inf.exists() ) {
        qWarning() << path << "No such file or directory";
        return QString::null;
    }

    inf.makeAbsolute();
    qDebug() << "File: " << inf.absolutePath();
    QDir dir = inf.dir();
    qDebug() << "Dir: " << dir.absolutePath();
    dir.makeAbsolute();

    while( dir.cdUp() ) {
        qDebug() << "Dir: " << dir.absolutePath();
    }

    QString la = TvInfo( dir.absolutePath() );

    if ( ! la.isNull() && ! la.isEmpty() ) {
        return la;
    }

    return QString::null;
}

#endif
