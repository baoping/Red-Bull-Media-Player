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
#include "helper.h"

#include <QDir>
#include <QByteArray>
#include <QSysInfo>

Helper::Helper() {
}

bool Helper::mkDirIfNotExists( QString dir ) {
    QDir directory = QDir( dir );
    return !directory.exists() && directory.mkpath( directory.absolutePath() );
}

bool Helper::rmDirIfExists( QString dir ) {
    QDir directory = QDir( dir );
    return directory.exists() && directory.rmpath( directory.absolutePath() );
}

QUrl Helper::decodeUrl( QString Url ) {
    QByteArray urlArray = Url.toLatin1();
    return QUrl::fromEncoded( urlArray, QUrl::TolerantMode );
}

QString Helper::getOsName() {
    ;
#ifdef Q_WS_X11
    return QString( "Linux" );
#endif
#ifdef Q_WS_MAC
    return QString( "MacOSX" );
#endif
#ifdef Q_WS_WIN
    return QString( "Windows" );
#endif
}

QString Helper::getOsVersion() {
#ifdef Q_WS_WIN

    switch( QSysInfo::WindowsVersion ) {
        case QSysInfo::WV_2000:
            return "WV_2000";

        case QSysInfo::WV_XP:
            return "WV_XP";

        case QSysInfo::WV_VISTA:
            return "WV_VISTA";

        case QSysInfo::WV_WINDOWS7:
            return "WV_WINDOWS7";

        default:
            return "WV_UNKOWN";
    }

#endif
#ifdef Q_WS_MAC

    switch( QSysInfo::MacintoshVersion ) {
        case QSysInfo::MV_PANTHER:
            return "MV_PANTHER";

        case QSysInfo::MV_TIGER:
            return "MV_TIGER";

        case QSysInfo::MV_LEOPARD:
            return "MV_LEOPARD";

        case QSysInfo::MV_SNOWLEOPARD:
            return "MV_SNOWLEOPARD";

        default:
            return "MV_UNKOWN";
    }

#endif
#ifdef Q_WS_X11
    return QString( "LV_UNKOWN" );
#endif
}
