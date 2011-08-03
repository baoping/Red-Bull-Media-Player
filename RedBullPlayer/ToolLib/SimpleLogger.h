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
#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#include <QString>

namespace RedBullPlayer {
    namespace Tools {
        class SimpleLogger {
            public:
                static SimpleLogger* instance();

                void info( QString loggerName, QString message );
                void error( QString loggerName, QString message );
                void fatal( QString loggerName, QString message );
                void debug( QString loggerName, QString message );
                void warn( QString loggerName, QString message );

                void enable();
                void disable();

                void setStartedMark();

            private:
                void log( int level, QString loggerName, QString message );

                bool _enabled;

                SimpleLogger();

                static SimpleLogger* _instance;
        };
    }
}

#endif // SIMPLELOGGER_H
