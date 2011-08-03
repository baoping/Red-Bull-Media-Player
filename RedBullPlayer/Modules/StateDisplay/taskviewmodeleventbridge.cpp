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
#include "taskviewmodeleventbridge.h"

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/StateDisplay/StatusChangedEvent.h"
#include "../../Events/EventTypes.h"
#include "../../Events/StateDisplay/RequestHoverBoxChangeVisibilityEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"


using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

TaskViewModelEventBridge::TaskViewModelEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, ITaskModel* model, QObject *parent ) :  QObject( parent ) {
    _model = model;
    _purpose = purpose;
    Aggregator::instance()->registerListener( TaskRepositoryTaskAdded, this );
    Aggregator::instance()->registerListener( TaskRepositoryTaskRemoved, this );
    Aggregator::instance()->registerListener( TaskStarted, this );
    Aggregator::instance()->registerListener( TaskProgress, this );
}

bool TaskViewModelEventBridge::event( QEvent* event ) {
    if ( event->type() == TaskRepositoryTaskAdded ) {
        onTaskRepositoryTaskAdded( static_cast<TaskRepositoryTaskAddedEvent*>( event ) );
    } else if ( event->type() == TaskRepositoryTaskRemoved ) {
        onTaskRepositoryTaskRemoved( static_cast<TaskRepositoryTaskRemovedEvent*>( event ) );
    } else if ( event->type() == TaskStarted ) {
        onTaskStarted( static_cast<TaskStartedEvent*>( event ) );
    } else if( event->type() == TaskProgress ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "ProgresEvent received" );
        onTaskProgress( static_cast<TaskProgressEvent*>( event ) );
    }

    return false;
}

void TaskViewModelEventBridge::onTaskRepositoryTaskAdded( TaskRepositoryTaskAddedEvent* e ) {
    if ( e->purpose() == _purpose )
        _model->add( e->taskInfo() );
}

void TaskViewModelEventBridge::onTaskRepositoryTaskRemoved( TaskRepositoryTaskRemovedEvent* e ) {
    _model->remove( e->taskId() );
}

void TaskViewModelEventBridge::onTaskStarted( RedBullPlayer::Events::TaskStartedEvent* e ) {
    _model->setStarted( e->taskId() );
}

void TaskViewModelEventBridge::onTaskProgress( TaskProgressEvent* e ) {
    _model->setProgress( e->taskId(), e->progress() );
}
