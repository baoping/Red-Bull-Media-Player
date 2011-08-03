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
#include "taskrepository.h"

#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"

using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;

TaskRepository::TaskRepository( QObject *parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "TaskRepository: Create New Repository." );
    bootstrap();
}

TaskRepository::~TaskRepository() {
    QList<QQueue<ITask*>*> values =  _taskQueueCollection->values();

    while ( !values.empty() ) {
        QQueue<ITask*>* queue = values.takeFirst();

        while ( !queue->isEmpty() ) {
            ITask* task = queue->takeFirst();
            task->deleteLater();
        }

        delete queue;
    }

    delete _taskQueueCollection;
}

void TaskRepository::bootstrap() {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "TaskRepository: Bootstrap Repository." );
    _taskQueueCollection = new QHash<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, QQueue<ITask*>*>();
}

bool TaskRepository::event( QEvent* event ) {
    return false;
}

/**
 * Returns the count of all tasks in repository regardless of queue
 *
 * \return count of all tasks
 */
int TaskRepository::count() {
    QList<QQueue<ITask*>*> values =  _taskQueueCollection->values();
    int count = 0;

    for( int i = 0; i < values.count(); i++ ) {
        count += values.at( i )->count();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepository: Count: %1." ).arg( count ) );
    return count;
}

/**
 * Returns the count of tasks for a given purpose
 *
 * \return count of tasks
 */
int TaskRepository::count( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    int count = 0;

    if ( _taskQueueCollection->contains( purpose ) ) {
        count = _taskQueueCollection->value( purpose )->count();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepository: Count(%1): %2." ).arg( purpose ).arg( count ) );
    return count;
}

/**
 * Enqueue task to proper Queue.
 * Creates the Queue if not exists.
 *
 */
void TaskRepository::enqueue( ITask* task ) {
    QQueue<ITask*>* queue = NULL;

    if ( !_taskQueueCollection->contains( task->purpose()->value() ) ) {
        queue = addQueueForPurpose( task->purpose()->value() );
    } else {
        queue = _taskQueueCollection->value( task->purpose()->value() );
    }

    queue->enqueue( task );
    task->setState( TaskState::PENDING );
    //emit signal
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepository: Enqueue Task: %1|%2." ).arg( task->taskInfo().first ).arg( task->taskInfo().second ) );
    emit taskAdded( task->purpose()->value(), task->taskInfo() );
}

/**
 * Dequeue task from proper Queue.
 *
 * \return the Task
 */
ITask* TaskRepository::next( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    if ( _taskQueueCollection->contains( purpose ) ) {
        QQueue<ITask*>* queue = _taskQueueCollection->value( purpose );
        ITask* task = queue->first();
        //emit signal
        return task;
    }

    return 0;
}


/**
 * Creates Queue for given purpose and add it to the collection
 *
 * \return a new empty Queue
 */
QQueue<ITask*>* TaskRepository::addQueueForPurpose( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    QQueue<ITask*>* queue = new QQueue<ITask*>();
    _taskQueueCollection->insert( purpose, queue );
    return queue;
}

/**
 * Gets a TaskDto by an given id
 *
 * \return the task or 0
 */
TaskInfo TaskRepository::get( QUuid taskId, bool* ok ) {
    ITask* task = getTask( taskId );
    *ok = ( task != 0 );
    return ( task != 0 ) ? task->taskInfo() : TaskInfo();
}

/**
 * Gets a TaskDto by an given id and given purpose
 *
 * \return the task or 0
 */
TaskInfo TaskRepository::get( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, bool* ok ) {
    ITask* task = getTask( taskId, purpose );
    *ok = ( task != 0 );
    return ( task != 0 ) ? task->taskInfo() : TaskInfo();
}

/**
 * Gets a Task by an given id
 *
 * \return the task or 0
 */
ITask* TaskRepository::getTask( QUuid taskId ) {
    QList<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum> keyList = _taskQueueCollection->keys();
    ITask* task = 0;

    for( int i = 0; i < keyList.count(); i++ ) {
        task = getTask( taskId, keyList.at( i ) );

        if ( task != 0 )
            break;
    }

    return task;
}

/**
 * Gets a Task by an given id and given purpose
 *
 * \return the task or 0
 */
ITask* TaskRepository::getTask( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    ITask* task = 0;

    if ( _taskQueueCollection->contains( purpose ) ) {
        task = findInQueue( taskId,
                            _taskQueueCollection->value( purpose ) );
    }

    return task;
}

/**
 * Search the given queue for a given id
 *
 * \return the task or 0
 */
ITask* TaskRepository::findInQueue( QUuid taskId, QQueue<ITask*>* queue ) {
    foreach( ITask * task, *queue ) {
        if ( task->guid() == taskId ) {
            return task;
        }
    }
    return 0;
}

/**
 * Remove a task from the given queue for a given id
 *
 */
bool TaskRepository::removeFromQueue( QUuid taskId, QQueue<ITask*>* queue ) {
    foreach( ITask * task, *queue ) {
        if ( task->guid() == taskId ) {
            queue->removeAll( task );
            // emit signal
            emit taskRemoved( taskId );
            return true;
        }
    }
    return false;
}

/**
 * Search the given queue for a given id
 *
 * \return true or false
 */
bool TaskRepository::contains( QUuid taskId ) {
    return ( getTask( taskId ) != 0 );
}

/**
 * Search the given queue for a given id and given purpose
 *
 * \return true or false
 */
bool TaskRepository::contains( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    return ( getTask( taskId, purpose ) != 0 );
}

/**
 * Removes a task from the queue
 *
 */
bool TaskRepository::remove( QUuid taskId ) {
    QList<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum> keyList = _taskQueueCollection->keys();

    for( int i = 0; i < keyList.count(); i++ ) {
        if ( remove( taskId, keyList.at( i ) ) ) {
            return true;
        }
    }

    return false;
}

/**
 * Removes a task from the queue
 *
 */
bool TaskRepository::remove( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    if ( _taskQueueCollection->contains( purpose ) ) {
        return removeFromQueue( taskId,
                                _taskQueueCollection->value( purpose ) );
    }

    return false;
}



