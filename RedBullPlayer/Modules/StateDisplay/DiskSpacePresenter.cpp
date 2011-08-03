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
#include "DiskSpacePresenter.h"

#include <QDesktopServices>
#include <QDir>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/UuidCreator.h"

#include "TaskViewFactory.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

DiskSpacePresenter::DiskSpacePresenter( IStateView* view, IDiskSpaceModel* model, ITaskViewFactory* taskViewFactory, QObject *parent )
    : IDiskSpacePresenter( parent ) {
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    _viewGuid = RedBullPlayer::Tools::UuidCreator::create();
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "DiskSpacePresenter: Create new Presenter: %1" )
                                     .arg( _guid ) );
    _view = view;
    _model = model;
    _taskViewFactory = taskViewFactory;
    TaskInfo taskInfo;
    taskInfo.first = _viewGuid;
    taskInfo.second = QString( "%1 of %2 free." ).arg( _model->freeSpaceString() ).arg( _model->maximumSpaceString() );
    ITaskView* taskView = _taskViewFactory->createViewFor( taskInfo, _view );
    taskView->hideCancelButton();
    _view->append( taskView );
    bootstrap();
}

DiskSpacePresenter::~DiskSpacePresenter() {
}

void DiskSpacePresenter::bootstrap() {
    connect( _view, SIGNAL( cancelAllClicked() ), this, SLOT( cancelAllClicked() ) );
    connect( _view, SIGNAL( mouseEnter() ), this, SLOT( mouseEnterView() ) );
    connect( _view, SIGNAL( mouseLeave() ), this, SLOT( mouseLeaveView() ) );
    _viewTimer = new QTimer( this );
    _viewTimer->setSingleShot( true );
    connect( _viewTimer, SIGNAL( timeout() ), this, SLOT( viewTimerTimeout() ) );
    _viewTimer->setInterval( Constants::STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT );
}

void DiskSpacePresenter::viewTimerTimeout() {
    _view->hide();
}

void DiskSpacePresenter::mouseEnterView() {
    if ( _viewTimer->isActive() )
        _viewTimer->stop();
}

void DiskSpacePresenter::mouseLeaveView() {
    if ( !_viewTimer->isActive() )
        _viewTimer->start();
}

void DiskSpacePresenter::showView( int x, int y ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "DiskSpacePresenter (%1) show View." )
                                     .arg( _guid ) );
    _view->updateText( _viewGuid, QString( "%1 of %2 free." ).arg( _model->freeSpaceString() ).arg( _model->maximumSpaceString() ) );
    _view->show( x, y );
    _viewTimer->start();
}

void DiskSpacePresenter::hideView() {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "DiskSpacePresenter (%1) hide View." )
                                     .arg( _guid ) );
    _view->hide();
}

bool DiskSpacePresenter::isViewVisible() {
    return _view->isVisible();
}

void DiskSpacePresenter::removeView( QUuid taskId ) {
    _view->remove( taskId );
}


void DiskSpacePresenter::cancelAllClicked() {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "DiskSpacePresenter (%1) Show Media clicked." )
                                     .arg( _guid ) );
    QDir dir( ConfigManager::instance()->getMediaDirectory() );
    QString path = dir.absolutePath();
    path = path.prepend( "file:///" );
    QUrl url( path, QUrl::TolerantMode );
    QDesktopServices::openUrl( url );
    hideView();
}

