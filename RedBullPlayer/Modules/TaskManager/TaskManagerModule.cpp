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
#include "TaskManagerModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QFuture>
#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/TaskManager/TaskStartedEvent.h"
#include "../../Events/TaskManager/TaskFinishedEvent.h"
#include "../../Events/TaskManager/TaskCanceledEvent.h"

using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;

TaskManagerModule::TaskManagerModule() : ITaskManager() {
}

TaskManagerModule::~TaskManagerModule() {
    if ( _executionTimer->isActive() )
        _executionTimer->stop();

    _executionTimer->deleteLater();
    TaskExecutor* downloadExecutor = _executorList->take( TaskPurpose::DOWNLOAD );
    downloadExecutor->deleteLater();
    TaskExecutor* ioExecutor = _executorList->take( TaskPurpose::IO );
    ioExecutor->deleteLater();
    TaskExecutor* updateExecutor = _executorList->take( TaskPurpose::UPDATE );
    updateExecutor->deleteLater();
    _repositoryEventBridge->deleteLater();
    _repository->deleteLater();
    delete _executorList;
}

void TaskManagerModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "Setup Module." );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    bootstrap();
}

void TaskManagerModule::bootstrap() {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "Bootstrap." );
    _executorList = new QHash<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, TaskExecutor*>();
    _executorList->insert( TaskPurpose::DOWNLOAD, createExecutor() );
    _executorList->insert( TaskPurpose::IO, createExecutor() );
    _executorList->insert( TaskPurpose::UPDATE, createExecutor() );
    _repository = new TaskRepository( this );
    _repositoryEventBridge = new TaskRepositoryEventBridge( _repository, Aggregator::instance(), this );
    _executionTimer = new QTimer( this );
    _executionTimer->setInterval( 3000 );
    connect( _executionTimer, SIGNAL( timeout() ), this, SLOT( onExecutionTimer() ) );
    Aggregator::instance()->registerListener( TaskCancelRequest, this );
    _executionTimer->start();
}

bool TaskManagerModule::event( QEvent* event ) {
    if ( event->type() == TaskCancelRequest ) {
        onTaskCancelRequest( static_cast<TaskCancelRequestEvent*>( event ) );
    }

    return false;
}

void TaskManagerModule::onTaskCancelRequest( TaskCancelRequestEvent* e ) {
    //check each executor
    QList<TaskPurpose::PurposeEnum> keys = _executorList->keys();

    for( int i = 0; i < keys.count(); i++ ) {
        TaskExecutor* executor = _executorList->value( keys.at( i ) );

        if ( executor != 0 ) {
            if ( executor->isRunning() ) {
                if ( executor->task()->guid() == e->taskId() ) {
                    executor->cancel();
                    return;
                }
            }
        }
    }
}

void TaskManagerModule::cleanUp() {
    // QMessageBox::information( 0, Q_FUNC_INFO, Q_FUNC_INFO );
    _executionTimer->stop();
    TaskExecutor* downloadExecutor = _executorList->value( TaskPurpose::DOWNLOAD );
    downloadExecutor->blockSignals( true );
    downloadExecutor->cleanUp();
    TaskExecutor* ioExecutor = _executorList->value( TaskPurpose::IO );
    ioExecutor->blockSignals( true );
    ioExecutor->cleanUp();
    TaskExecutor* updateExecutor = _executorList->value( TaskPurpose::UPDATE );
    updateExecutor->blockSignals( true );
    updateExecutor->cleanUp();
}

void TaskManagerModule::onExecutionTimer() {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "onExecutionTimer." );
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "tryExecute (DOWNLOAD)." );
    tryExecute( TaskPurpose::DOWNLOAD );
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "tryExecute (IO)." );
    tryExecute( TaskPurpose::IO );
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, "tryExecute (UPDATE)." );
    tryExecute( TaskPurpose::UPDATE );
}

TaskExecutor* TaskManagerModule::createExecutor() {
    TaskExecutor* executor = new TaskExecutor( this );
    connect( executor, SIGNAL( taskFinished( QUuid ) ), this, SLOT( taskFinished( QUuid ) ) );
    connect( executor, SIGNAL( taskCanceled( QUuid ) ), this, SLOT( taskCanceled( QUuid ) ) );
    connect( executor, SIGNAL( taskRunning( QUuid ) ), this, SLOT( taskRunning( QUuid ) ) );
    return executor;
}

bool TaskManagerModule::isExecutorRunning( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    bool isRunning = false;

    if ( _executorList->contains( purpose ) ) {
        isRunning = _executorList->value( purpose )->isRunning();
        SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "Check for Executor isRunning: %1" ).arg( isRunning ) );
    }

    return isRunning;
}

void TaskManagerModule::tryExecute( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    if ( !isExecutorRunning( purpose ) ) { //executor is pending
        if ( _repository->count( purpose ) > 0 ) {
            ITask* task = _repository->next( purpose );

            if ( task != 0 ) {
                SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "Execute next Task." ) );
                _executorList->value( purpose )->execute( task );
            }
        }
    }
}

void TaskManagerModule::taskFinished( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "Task finished: %1" ).arg( taskId ) );
    TaskFinishedEvent ev( taskId );
    Aggregator::instance()->sendEvent( &ev );
    _repository->remove( taskId );
}

void TaskManagerModule::taskCanceled( QUuid taskId ) {
    qDebug() << "TaskManagerModule => TaskCanceled(" << taskId.toString() << ")";
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "Task canceled: %1" ).arg( taskId ) );
    TaskCanceledEvent ev( taskId );
    Aggregator::instance()->sendEvent( &ev );
    _repository->remove( taskId );
}

void TaskManagerModule::taskRunning( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TASKMANAGER, QString( "Task running: %1" ).arg( taskId ) );
    TaskStartedEvent ev( taskId );
    Aggregator::instance()->sendEvent( &ev );
}

Q_EXPORT_PLUGIN2( TaskManagerModule, RedBullPlayer::Modules::TaskManager::TaskManagerModule );

