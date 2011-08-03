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
#include "PathResolver.h"


// QT includes
#include <QDir>
#include <QFile>
#include <QDebug>

// Project includes
#include "../../Container/Constants.h"
#include "../SimpleLogger.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

QString PathResolver::createResolvablePath( QString absoluteFilePath, QString absoluteBaseDirectory, QString basedirPlaceHolder ) {
    //    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
    //                                     QString("PathResolver::createResolvablePath: %1 %2 %3")
    //                                     .arg(absoluteFilePath)
    //                                     .arg(absoluteBaseDirectory)
    //                                     .arg(basedirPlaceHolder));
    if ( ! QDir::isAbsolutePath( absoluteBaseDirectory ) ) {
        //  qWarning() << "Invalid argument: absoluteBaseDirectory - not an absolute path";
        SimpleLogger::instance()->warn( Constants::LOGGER_TOOLLIB, "Invalid argument: absoluteBaseDirectory - not an absolute path" );
        return absoluteFilePath;
    }

    if ( ! QDir::isAbsolutePath( absoluteFilePath ) ) {
        //  qWarning() << "Invalid argument: absoluteFilePath - not an absolute path";
        SimpleLogger::instance()->warn( Constants::LOGGER_TOOLLIB, "Invalid argument: absoluteFilePath - not an absolute path" );
        return absoluteFilePath;
    }

    if ( absoluteFilePath.startsWith( absoluteBaseDirectory ) ) {
        return absoluteFilePath.replace( absoluteBaseDirectory, basedirPlaceHolder );
    } else {
        SimpleLogger::instance()->warn( Constants::LOGGER_TOOLLIB, "Invalid argument: baseDir is part of filePath" );
        //  qWarning() << "Invalid argument: baseDir is part of filePath";
        return absoluteFilePath;
    }
}


QString PathResolver::resolvePath( QString resolvablePath, QString absoluteBaseDirectory,  QString basedirPlaceHolder ) {
    //    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
    //                                     QString("PathResolver::resolvePath: %1 %2 %3")
    //                                     .arg(resolvablePath)
    //                                     .arg(absoluteBaseDirectory)
    //                                     .arg(basedirPlaceHolder));
    if ( ! QDir::isAbsolutePath( absoluteBaseDirectory ) ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_TOOLLIB, "Invalid argument: absoluteBaseDirectory - not an absolute path" );
        // qWarning() << "Invalid argument: absoluteBaseDirectory - not an absolute path";
        return resolvablePath;
    }

    if ( ! resolvablePath.startsWith( basedirPlaceHolder ) ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_TOOLLIB, "Invalid argument: resolvablePath not starting with placeholder" );
        // qWarning() << "Invalid argument: resolvablePath not starting with placeholder";
        return resolvablePath;
    }

    return QDir::cleanPath( absoluteBaseDirectory + resolvablePath.replace( basedirPlaceHolder, "/" ) );
}
