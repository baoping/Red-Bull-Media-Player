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
#include "taskexecutor.h"

#include <QtConcurrentMap>
#include <QtConcurrentRun>
#include <QFuture>
#include <QList>
#include <QDebug>

#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

TaskExecutor::TaskExecutor( QObject *parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "Create new TaskExecutor." );
    _currentTask = 0;
    _running = false;
}

TaskExecutor::~TaskExecutor() {
    _currentTask = 0;
}

void TaskExecutor::execute( ITask* task ) {
    _running = true;
    _currentTask = task;
    connect( _currentTask, SIGNAL( finished( bool ) ), this, SLOT( task_finished( bool ) ) );
    connect( _currentTask, SIGNAL( prepared( bool ) ), this, SLOT( task_prepared( bool ) ) );
    connect( _currentTask, SIGNAL( canceled() ), this, SLOT( task_canceled() ), Qt::QueuedConnection );
    task->setState( TaskState::RUNNING );
    emit taskRunning( task->guid() );
    task->prepare();
}

void TaskExecutor::task_finished( bool success ) {
    if ( !success ) {
        _currentTask->setState( TaskState::EXECUTED );
        _currentTask->setResult( TaskResult::FAILED );
    } else {
        _currentTask->setState( TaskState::EXECUTED );
        _currentTask->setResult( TaskResult::SUCCESSFUL );
    }

    _currentTask->clean();
    _running = false;
    emit taskFinished( _currentTask->guid() );
}

void TaskExecutor::task_prepared( bool success ) {
    if ( !success ) {
        task_canceled();
    } else {
        _currentTask->execute();
    }
}

void TaskExecutor::task_canceled() {
    qDebug() << "TaskExecutor => TaskCanceled";
    _currentTask->setState( TaskState::CANCELED );
    _currentTask->setResult( TaskResult::FAILED );
    _running = false;
    emit taskCanceled( _currentTask->guid() );
}

void TaskExecutor::cancel() {
    qDebug() << "TaskExecutor => TaskCancel";

    if ( _currentTask != 0 ) {
        _currentTask->cancel();
    }

    _running = false;
}

void TaskExecutor::cleanUp() {
    if ( _currentTask ) {
        _currentTask->cancel();
    }

    _running = false;
}


