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
#include "UpdateProcess.h"

#include <QCoreApplication>

// Project includes
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Updater;

UpdateProcess::UpdateProcess( QString fromVersion, QString toVersion, QObject* parent ) : QObject( parent ) {
    _fromVersion = new PlayerVersion( fromVersion, this );
    _toVersion = new PlayerVersion( toVersion, this );
    _tasks.clear();
}


void UpdateProcess::execute() {
    for ( int i = 0; i < _tasks.size(); ++i ) {
        qApp->processEvents();
        _tasks.at( i )->execute();
        qApp->processEvents();
    }
}

void UpdateProcess::addTask( UpdateTask* task ) {
    _tasks.append( task );
    connect( task, SIGNAL( taskFailed( QString, QString ) ),
             this, SIGNAL( taskFailed( QString, QString ) ) );
    connect( task, SIGNAL( taskFinished( QString ) ),
             this, SIGNAL( taskFinished( QString ) ) );
    connect( task, SIGNAL( taskStarted( QString ) ),
             this, SIGNAL( taskStarted( QString ) ) );
}

// Static
bool UpdateProcess::lessThen( UpdateProcess* first,  UpdateProcess* second ) {
    return *first->fromVersion() < *second->fromVersion();
}
