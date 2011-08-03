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
#include "SimpleLogger.h"

//log4qt
#include "log4qt/basicconfigurator.h"
#include "log4qt/consoleappender.h"
#include "log4qt/dailyrollingfileappender.h"
#include "log4qt/fileappender.h"
#include "log4qt/helpers/configuratorhelper.h"
#include "log4qt/helpers/datetime.h"
#include "log4qt/helpers/factory.h"
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/helpers/patternformatter.h"
#include "log4qt/helpers/properties.h"
#include "log4qt/logmanager.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/patternlayout.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/rollingfileappender.h"
#include "log4qt/simplelayout.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/varia/denyallfilter.h"
#include "log4qt/varia/levelmatchfilter.h"
#include "log4qt/varia/levelrangefilter.h"
#include "log4qt/varia/stringmatchfilter.h"

#include "../Container/Constants.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

SimpleLogger* SimpleLogger::_instance = 0;

SimpleLogger::SimpleLogger() {
    _enabled = false;
}

SimpleLogger* SimpleLogger::instance() {
    if ( _instance == 0 ) {
        _instance = new SimpleLogger();
    }

    Q_ASSERT( _instance != 0 );
    return _instance;
}

void SimpleLogger::enable() {
    _enabled = true;
}

void SimpleLogger::disable() {
    _enabled = false;
}

void SimpleLogger::log( int level, QString loggerName, QString message ) {
    if ( !_enabled )
        return;

    QString currentLoggerName = Constants::LOGGER_ROOT;

    if ( Constants::KNOWN_LOGGER.contains( loggerName, Qt::CaseInsensitive ) ) {
        currentLoggerName = loggerName;
    }

    switch( level ) {
        case 1 :
            Log4Qt::Logger::logger( currentLoggerName )->info( message );
            break;

        case 2 :
            Log4Qt::Logger::logger( currentLoggerName )->warn( message );
            break;

        case 3 :
            Log4Qt::Logger::logger( currentLoggerName )->error( message );
            break;

        case 4 :
            Log4Qt::Logger::logger( currentLoggerName )->fatal( message );
            break;

        case 5 :
            Log4Qt::Logger::logger( currentLoggerName )->debug( message );
            break;
    }
}

void SimpleLogger::info( QString loggerName, QString message ) {
    log( 1, loggerName, message );
}

void SimpleLogger::warn( QString loggerName, QString message ) {
    log( 2, loggerName, message );
}

void SimpleLogger::error( QString loggerName, QString message ) {
    log( 3, loggerName, message );
}

void SimpleLogger::fatal( QString loggerName, QString message ) {
    log( 4, loggerName, message );
}

void SimpleLogger::debug( QString loggerName, QString message ) {
    log( 5, loggerName, message );
}

void SimpleLogger::setStartedMark() {
    int i;

    for( i = 0; i < Constants::KNOWN_LOGGER.count(); i++ ) {
        _instance->debug( Constants::KNOWN_LOGGER.at( i ), "---------- LOGGER STARTED ----------" );
    }
}


