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
#include "FileSystem.h"

// Qt includes
#include <QDir>
#include <QDebug>
#include <QFileInfo>

// Project includes
#ifndef SELFEXTRACT
#include "../../Container/Constants.h"
#include "../SimpleLogger.h"
using namespace RedBullPlayer::Container;
#endif

using namespace RedBullPlayer::Tools;

FileSystem::FileSystem( QString path ) {
    Q_ASSERT( ! path.isNull() && ! path.isEmpty() );

    if ( ! QFileInfo( path ).exists() ) {
        qWarning() <<  path << "does not exist!";
#ifndef SELFEXTRACT
        SimpleLogger::instance()->error( Constants::LOGGER_TOOLLIB, QString( "%1 does not exist!" ).arg( path ) );
#endif
    }

    _path = path;
}

quint64 FileSystem::capacity() {
    if ( ! update() ) {
#ifndef SELFEXTRACT
        SimpleLogger::instance()->error( Constants::LOGGER_TOOLLIB, QString( "Could not calculate FileSystem capacaty!" ) );
#endif
        qWarning() << "Could not calculate FileSystem capacaty!";
        return 0;
    }

    return _capacaty;
}

quint64 FileSystem::usedBytes() {
    if ( ! update() ) {
#ifndef SELFEXTRACT
        SimpleLogger::instance()->error( Constants::LOGGER_TOOLLIB, QString( "Could not calculate FileSystem capacaty!" ) );
#endif
        qWarning() << "Could not calculate FileSystem capacaty!";
        return 0;
    }

    return _usedBytes;
}

quint64 FileSystem::freeBytes() {
    if ( ! update() ) {
#ifndef SELFEXTRACT
        SimpleLogger::instance()->error( Constants::LOGGER_TOOLLIB, QString( "Could not calculate FileSystem capacaty!" ) );
#endif
        qWarning() << "Could not calculate FileSystem capacaty!";
        return 0;
    }

    return _freeBytes;
}
