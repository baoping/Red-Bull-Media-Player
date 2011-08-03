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
#include "StateDisplayModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>

#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "TaskViewFactory.h"
#include "StateViewFactory.h"
#include "TaskPresenter.h"
#include "TaskModel.h"
#include "DiskSpaceModel.h"
#include "DiskSpacePresenter.h"
#include "InternetAvailabilityModel.h"
#include "InternetAvailabilityPresenter.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::StateDisplay;

StateDisplayModule::StateDisplayModule() : IStateDisplay() {
}

void StateDisplayModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Setup Module." );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    bootstrap();
}

void StateDisplayModule::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void StateDisplayModule::bootstrap() {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Resolve StyleFactory." );
    resolveStyleFactory();
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Bootstrap." );
    QString stateDisplayDownloadHeader = ConfigManager::instance()->stateDisplayDownloadHeader();
    QString stateDisplayIOHeader = ConfigManager::instance()->stateDisplayIOHeader();
    QString stateDisplayUpdateHeader = ConfigManager::instance()->stateDisplayUpdateHeader();
    QString stateDisplayDiskSpaceHeader = ConfigManager::instance()->stateDisplayDiskSpaceHeader();
    QString stateDisplayInternetAvailabilityHeader = ConfigManager::instance()->stateDisplayInternetAvailabilityHeader();
    //ViewFactories
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Create ViewFactories." );
    _stateViewFactory = new StateViewFactory( this );
    _taskViewFactory = new TaskViewFactory( this );
    //Set TextStyles to Factory for Views
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Get TextStyles for Views." );
    bool ok;
    ITextStyle* stateViewTextStyle = _styleFactory->getTextStyle( "StateView_HeaderText", &ok );

    if ( ok ) {
        _stateViewFactory->setTextStyle( stateViewTextStyle );
    }

    ITextStyle* taskViewTextStyle = _styleFactory->getTextStyle( "TaskView_ListText", &ok );

    if ( ok ) {
        _taskViewFactory->setTextStyle( taskViewTextStyle );
    }

    //Models
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Create ViewModels." );
    _ioModel = new TaskModel( this );
    _ioModelEventBridge = new TaskViewModelEventBridge( TaskPurpose::IO, _ioModel, this );
    _updateModel = new TaskModel( this );
    _updateModelEventBridge = new TaskViewModelEventBridge( TaskPurpose::UPDATE, _updateModel, this );
    _diskSpaceModel = new DiskSpaceModel( this );
    _internetAvailabilityModel = new InternetAvailabilityModel( this );
    //Create StateViews
    IStateView* ioStateView = _stateViewFactory->createViewFor( TaskPurpose::IO, stateDisplayIOHeader, _shell );
    IStateView* updateStateView = _stateViewFactory->createViewFor( TaskPurpose::UPDATE, stateDisplayUpdateHeader, _shell );
    IStateView* diskSpaceView = _stateViewFactory->createViewFor( TaskPurpose::DISK_SPACE, stateDisplayDiskSpaceHeader, _shell );
    IStateView *internetAvailabilityView = _stateViewFactory->createViewFor( TaskPurpose::INTERNET_AVAILABILITY, stateDisplayInternetAvailabilityHeader, _shell );
    //Presenter
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY, "Create Presenter." );
    _updatePresenter = new TaskPresenter(
        updateStateView,
        _updateModel,
        _taskViewFactory,
        this );
    _updatePresenterEventBridge = new TaskPresenterEventBridge( TaskPurpose::UPDATE, _updatePresenter, this );
    _ioPresenter = new TaskPresenter(
        ioStateView,
        _ioModel,
        _taskViewFactory,
        this );
    _ioPresenterEventBridge = new TaskPresenterEventBridge( TaskPurpose::IO, _ioPresenter, this );
    _diskSpacePresenter = new DiskSpacePresenter(
        diskSpaceView,
        _diskSpaceModel,
        _taskViewFactory,
        this );
    _diskSpaceEventBridge = new StatePresenterEventBridge( TaskPurpose::DISK_SPACE, _diskSpacePresenter, this );
    _internetAvailabilityPresenter = new InternetAvailabilityPresenter( internetAvailabilityView, _internetAvailabilityModel,
            _taskViewFactory, this );
    _internetAvailabilityEventBridge = new InternetAvailabilityPresenterEventBridge( TaskPurpose::INTERNET_AVAILABILITY,
            _internetAvailabilityPresenter, this );
}

bool StateDisplayModule::event ( QEvent * e ) {
    return false;
}

Q_EXPORT_PLUGIN2( StateDisplayModule, RedBullPlayer::Modules::StateDisplay::StateDisplayModule );
