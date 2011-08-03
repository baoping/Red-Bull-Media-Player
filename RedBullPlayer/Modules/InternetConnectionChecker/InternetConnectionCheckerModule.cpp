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
#include "InternetConnectionCheckerModule.h"

// Qt includes
#include <QtPlugin>
#include <QDebug>

// Project includes
#include "../../ToolLib/ConfigManager.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"


using namespace RedBullPlayer::Modules::InternetConnectionChecker;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;

InternetConnectionCheckerModule::InternetConnectionCheckerModule() {}

void InternetConnectionCheckerModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    _lastState = false;
}

void InternetConnectionCheckerModule::SetupModuleConnections() {
    _statusProvider = new InternetConnectionStatusProvider(
        ConfigManager::instance()->referenceHost(),
        ConfigManager::instance()->checkPeriod(),
        this );
    connect( _statusProvider, SIGNAL( statusChanged( bool ) ), this, SLOT( statusChanged( bool ) ) );
    _statusProvider->start(); //initialize check & start timer
}

bool InternetConnectionCheckerModule::internetAvailable() {
    return _lastState;
}

void InternetConnectionCheckerModule::cleanUp() {
    if ( _statusProvider->isRunning() )
        _statusProvider->stop();

    _statusProvider->deleteLater();
}

void InternetConnectionCheckerModule::statusChanged( bool internetAvailable ) {
    _lastState = internetAvailable;
    InternetConnectionStatusChangedEvent e( internetAvailable );
    Aggregator::instance()->sendEvent( &e );
}

Q_EXPORT_PLUGIN2( InternetConnectionCheckerModule, RedBullPlayer::Modules::InternetConnectionChecker::InternetConnectionCheckerModule )
