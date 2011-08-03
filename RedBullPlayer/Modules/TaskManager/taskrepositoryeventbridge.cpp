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
#include "taskrepositoryeventbridge.h"

#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/TaskManager/TaskRepositoryTaskAddedEvent.h"
#include "../../Events/TaskManager/TaskRepositoryTaskRemovedEvent.h"
#include "../../Events/TaskManager/TaskCanceledEvent.h"

using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

TaskRepositoryEventBridge::TaskRepositoryEventBridge( TaskRepository* repository, Aggregator* eventAggregator, QObject *parent ) :
    QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "TaskRepositoryEventBridge: Create New EventBridge." );
    _repository = repository;
    _eventAggregator = eventAggregator;
    connect( _repository,
             SIGNAL( taskAdded( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, TaskInfo ) ),
             this,
             SLOT( taskAdded( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, TaskInfo ) ) );
    connect( _repository, SIGNAL( taskRemoved( QUuid ) ), this, SLOT( taskRemoved( QUuid ) ) );
    _eventAggregator->registerListener( TaskExecuteRequest, this );
    _eventAggregator->registerListener( TaskCancelRequest, this );
}

bool TaskRepositoryEventBridge::event( QEvent* event ) {
    if ( event->type() == TaskExecuteRequest ) {
        onTaskExecuteRequest( static_cast<TaskExecuteRequestEvent*>( event ) );
    } else if ( event->type() == TaskCancelRequest ) {
        onTaskCancelRequest( static_cast<TaskCancelRequestEvent*>( event ) );
    }

    return false;
}

void TaskRepositoryEventBridge::onTaskExecuteRequest( RedBullPlayer::Events::TaskExecuteRequestEvent* e ) {
    if ( !_repository->contains( e->task()->guid(), e->task()->purpose()->value() ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepositoryEventBridge: Enqueue Task: %1|%2." )
                                         .arg( e->task()->taskInfo().first )
                                         .arg( e->task()->taskInfo().second ) );
        _repository->enqueue( e->task() );
    }
}

void TaskRepositoryEventBridge::onTaskCancelRequest( RedBullPlayer::Events::TaskCancelRequestEvent* e ) {
    if ( _repository->contains( e->taskId() ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepositoryEventBridge: Cancel Task: %1." )
                                         .arg( e->taskId() ) );
        _repository->remove( e->taskId() );
        taskRemoved( e->taskId() );
        TaskCanceledEvent ev( e->taskId() );
        Aggregator::instance()->sendEvent( &ev );
    }
}

void TaskRepositoryEventBridge::taskAdded( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, TaskInfo taskInfo ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepositoryEventBridge: Task added to Repository: %1|%2|%3." )
                                     .arg( taskInfo.first )
                                     .arg( taskInfo.second )
                                     .arg( TaskPurpose::purposeString( purpose ) ) );
    TaskRepositoryTaskAddedEvent ev( purpose, taskInfo );
    _eventAggregator->sendEvent( &ev );
}

void TaskRepositoryEventBridge::taskRemoved( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "TaskRepositoryEventBridge: Task removed from Repository: %1." )
                                     .arg( taskId ) );
    TaskRepositoryTaskRemovedEvent ev( taskId );
    _eventAggregator->sendEvent( &ev );
}
