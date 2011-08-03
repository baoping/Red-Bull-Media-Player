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
#include "InternetAvailabilityPresenter.h"

#include "../../ToolLib/UuidCreator.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

InternetAvailabilityPresenter::InternetAvailabilityPresenter( IStateView* view, IInternetAvailabilityModel* model,
        ITaskViewFactory* taskViewFactory, QObject *parent ) {
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    _viewGuid = RedBullPlayer::Tools::UuidCreator::create();
    _view = view;
    _view->hideCancelAll();
    _model = model;
    _taskViewFactory = taskViewFactory;
    TaskInfo taskInfo;
    taskInfo.first = _viewGuid;
    taskInfo.second = QString( _model->offlineText() );
    ITaskView* taskView = _taskViewFactory->createViewFor( taskInfo, _view );
    taskView->hideCancelButton();
    _view->append( taskView );
    bootstrap();
}

void InternetAvailabilityPresenter::bootstrap() {
    connect( _view, SIGNAL( mouseEnter() ), this, SLOT( mouseEnterView() ) );
    connect( _view, SIGNAL( mouseLeave() ), this, SLOT( mouseLeaveView() ) );
    _viewTimer = new QTimer( this );
    _viewTimer->setSingleShot( true );
    connect( _viewTimer, SIGNAL( timeout() ), this, SLOT( viewTimerTimeout() ) );
    _viewTimer->setInterval( Constants::STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT );
}

void InternetAvailabilityPresenter::viewTimerTimeout() {
    _view->hide();
}

void InternetAvailabilityPresenter::mouseEnterView() {
    if ( _viewTimer->isActive() )
        _viewTimer->stop();
}

void InternetAvailabilityPresenter::mouseLeaveView() {
    if ( !_viewTimer->isActive() )
        _viewTimer->start();
}

void InternetAvailabilityPresenter::showView( int x, int y ) {
    _view->show( x, y );
    _viewTimer->start();
}

void InternetAvailabilityPresenter::hideView() {
    _view->hide();
}

bool InternetAvailabilityPresenter::isViewVisible() {
    return _view->isVisible();
}

void InternetAvailabilityPresenter::removeView( QUuid taskId ) {
    _view->remove( taskId );
}

void InternetAvailabilityPresenter::handleInternetAvailabilityChanged( bool internetAvailable ) {
    if( internetAvailable )
        _view->updateText( _viewGuid, _model->onlineText() );
    else
        _view->updateText( _viewGuid, _model->offlineText() );
}
