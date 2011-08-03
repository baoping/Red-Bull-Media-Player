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
#include "Version.h"

#include <QDebug>
#include <QString>
#include <QStringList>

Version::Version( QString versionString ) {
    parseString( versionString );
}

void Version::parseString( QString versionString ) {
    QStringList versionParts = versionString.split( ".", QString::SkipEmptyParts );
    _majorVersion = -1;
    _minorVersion = -1;
    _build = -1;
    _teamCityBuildNumber = -1;

    if ( versionParts.size() > 0 )
        _majorVersion = versionParts[0].toInt();

    if ( versionParts.size() > 1 )
        _minorVersion = versionParts[1].toInt();

    if ( versionParts.size() > 2 )
        _build = versionParts[2].toInt();

    if ( versionParts.size() > 3 )
        _teamCityBuildNumber = versionParts[3].toInt();

    //  QString v = versionString();
    QString ret = QString( "%1.%2.%3.%4" ).arg( _majorVersion ).arg( _minorVersion ).arg( _build ).arg( _teamCityBuildNumber );
    qDebug() << "Parsed Version: " << ret;
}

int Version::compare( Version version ) const {
    qDebug() << Q_FUNC_INFO << this->versionString() << " with: " << version.versionString();
    int c = compareEx( _majorVersion, version.majorVersion() );

    if ( c != 0 ) return c;

    c = compareEx( _minorVersion, version.minorVersion() );

    if ( c != 0 ) return c;

    c = compareEx( _build, version.build() );

    if ( c != 0 ) return c;

    c = compareEx( _teamCityBuildNumber, version.teamCityBuildNumber() );
    return c;
}

int Version::compareEx( int a, int b ) const {
    if ( a == -1 || b == -1 ) return 0;

    return ( a == b ) ? 0 : ( a < b ) ? -1 : ( a > b ) ? 1 : 0;
}

QString Version::versionString() const {
    QString ret = "%1.%2.%3.%4";
    return ret.arg( _majorVersion ).arg( _minorVersion ).arg( _build ).arg( _teamCityBuildNumber );
}
