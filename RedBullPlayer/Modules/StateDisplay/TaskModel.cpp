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
#include "TaskModel.h"

#include <QHashIterator>
#include <QDebug>

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Tools;

TaskModel::TaskModel( QObject* parent ) : ITaskModel ( parent ) {
    _tasks = new QHash<QUuid, QString>();
    _guid = RedBullPlayer::Tools::UuidCreator::create(); //identifier for object
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "Create new ViewModel: %1" )
                                     .arg( _guid ) );
}

TaskModel::~TaskModel() {
    _tasks->clear();
    delete _tasks;
}

TaskInfo TaskModel::get( QUuid taskId, bool* ok ) {
    *ok = _tasks->contains( taskId );
    TaskInfo taskInfo;
    taskInfo.first = taskId;

    if ( ok )
        taskInfo.second = _tasks->value( taskId );
    else
        taskInfo.second = "";

    return taskInfo;
}

QList<TaskInfo> TaskModel::all() {
    QList<QUuid> keys = _tasks->keys();
    QList<TaskInfo> taskInfos;

    for( int i = 0; i < keys.count(); i++ ) {
        TaskInfo taskInfo;
        taskInfo.first = keys.at( i );
        taskInfo.second = _tasks->value( keys.at( i ) );
        taskInfos.append( taskInfo );
    }

    return taskInfos;
}

void TaskModel::add( TaskInfo taskInfo ) {
    if ( _tasks->contains( taskInfo.first ) ) {
        return;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "ViewModel (%1) new task added: %2" )
                                     .arg( _guid )
                                     .arg( taskInfo.first ) );
    _tasks->insert( taskInfo.first, taskInfo.second );
    emit taskAdded( taskInfo.first );
}

void TaskModel::remove( QUuid taskId ) {
    if ( _tasks->contains( taskId ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                         QString( "ViewModel (%1) remove task: %2" )
                                         .arg( _guid )
                                         .arg( taskId ) );
        _tasks->remove( taskId );
        emit taskRemoved( taskId );
    }
}

void TaskModel::setStarted( QUuid taskId ) {
    if ( _tasks->contains( taskId ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                         QString( "ViewModel (%1) set started: %2" )
                                         .arg( _guid )
                                         .arg( taskId ) );
        emit taskStarted( taskId );
    }
}

void TaskModel::setProgress( QUuid taskId, int progress ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "Progress event:" ) );

    if ( _tasks->contains( taskId ) ) {
        emit taskProgress( taskId, progress );
    }
}

