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
#include "NewsTickerModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QGridLayout>
#include <QLayout>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Container.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerStoppedEvent.h"
#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"
#include "../../Interfaces/IXmlNewsFeed.h"


using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::NewsTicker;
using namespace RedBullPlayer::Events;

NewsTickerModule::NewsTickerModule() {
}

void NewsTickerModule::Setup( QObject* obj, QDomElement config ) {
    _shell = qobject_cast<Shell*>( obj );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
}

bool NewsTickerModule::event ( QEvent * e ) {
    return false;
}

Q_EXPORT_PLUGIN2( NewsTickerModule, RedBullPlayer::Modules::NewsTicker::NewsTickerModule )
