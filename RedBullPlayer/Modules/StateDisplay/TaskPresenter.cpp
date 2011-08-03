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
#include "TaskPresenter.h"


#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"
#include "TaskViewFactory.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;


TaskPresenter::TaskPresenter( IStateView* view, ITaskModel* model, ITaskViewFactory* taskViewFactory, QObject* parent ) : ITaskPresenter( parent ) {
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter: Create new Presenter: %1" )
                                     .arg( _guid ) );
    _view = view;
    _model = model;
    _taskViewFactory = taskViewFactory;
    bootstrap();
}

TaskPresenter::~TaskPresenter() {
}

void TaskPresenter::bootstrap() {
    connect( _model, SIGNAL( taskAdded( QUuid ) ), this, SLOT( taskAdded( QUuid ) ) );
    connect( _model, SIGNAL( taskRemoved( QUuid ) ), this, SLOT( taskRemoved( QUuid ) ) );
    connect( _model, SIGNAL( taskStarted( QUuid ) ), this, SLOT( taskStarted( QUuid ) ) );
    connect( _model, SIGNAL( taskProgress( QUuid, int ) ), this, SLOT( taskProgress( QUuid, int ) ) );
    connect( _view, SIGNAL( cancelClicked( QUuid ) ), this, SLOT( cancelClicked( QUuid ) ) );
    connect( _view, SIGNAL( cancelAllClicked() ), this, SLOT( cancelAllClicked() ) );
    connect( _view, SIGNAL( mouseEnter() ), this, SLOT( mouseEnterView() ) );
    connect( _view, SIGNAL( mouseLeave() ), this, SLOT( mouseLeaveView() ) );
    _viewTimer = new QTimer( this );
    _viewTimer->setSingleShot( true );
    connect( _viewTimer, SIGNAL( timeout() ), this, SLOT( viewTimerTimeout() ) );
    _viewTimer->setInterval( Constants::STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT );
}

void TaskPresenter::viewTimerTimeout() {
    _view->hide();
}

void TaskPresenter::mouseEnterView() {
    if ( _viewTimer->isActive() )
        _viewTimer->stop();
}

void TaskPresenter::mouseLeaveView() {
    if ( !_viewTimer->isActive() )
        _viewTimer->start();
}

void TaskPresenter::showView( int x, int y ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) show View." )
                                     .arg( _guid ) );
    _view->show( x, y );
    _viewTimer->start();
}

void TaskPresenter::hideView() {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) hide View." )
                                     .arg( _guid ) );
    _view->hide();
}

bool TaskPresenter::isViewVisible() {
    return _view->isVisible();
}

void TaskPresenter::removeView( QUuid taskId ) {
    _view->remove( taskId );

    if ( _model->count() == 1 ) {
        _view->hideCancelAll();
    } else if ( _model->count() == 0 ) {
        _view->hide();
        emit stateIconVisibilityChanged( false );
    }
}

void TaskPresenter::taskAdded( QUuid taskId ) {
    bool ok;
    TaskInfo taskInfo = _model->get( taskId, &ok );

    if ( ok ) {
        emit stateIconVisibilityChanged( true );

        if ( _taskViewFactory == NULL )
            SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                             QString( "TaskPresenter (%1) no TaskViewFactory." )
                                             .arg( _guid ) );

        ITaskView* taskView = _taskViewFactory->createViewFor( taskInfo, _view );

        if ( taskView != NULL ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                             QString( "TaskPresenter (%1) append TaskView." )
                                             .arg( _guid ) );
            connect( taskView, SIGNAL( cancelClicked( QUuid ) ), _view, SIGNAL( cancelClicked( QUuid ) ) );
            _view->append( taskView );
        }
    }
}

void TaskPresenter::taskRemoved( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) task Removed: %2" )
                                     .arg( _guid )
                                     .arg( taskId ) );
    removeView( taskId );
}

void TaskPresenter::taskStarted( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) task started: %2" )
                                     .arg( _guid )
                                     .arg( taskId ) );
    _view->activate( taskId );
}

void TaskPresenter::taskProgress( QUuid taskId, int progress ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "PROGRESS: (%1)" )
                                     .arg( progress ) );
    QString progressText = QString( "%0%" ).arg( progress );
    _view->updateProgressText( taskId, progressText );
}

void TaskPresenter::cancelClicked( QUuid taskId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) cancel clicked on view for task: %2" )
                                     .arg( _guid )
                                     .arg( taskId ) );
    emit requestTaskCancel( taskId );
}

void TaskPresenter::cancelAllClicked() {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenter (%1) cancel all clicked." )
                                     .arg( _guid ) );
    QList<TaskInfo> allTasks = _model->all();

    for( int i = 0; i < allTasks.count(); i++ ) {
        emit requestTaskCancel( allTasks.at( i ).first );
    }
}
