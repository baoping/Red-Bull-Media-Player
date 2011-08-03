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
#include "RootModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QLayout>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerStoppedEvent.h"
#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Root;
using namespace RedBullPlayer::Events;

RootModule::RootModule() {
}

void RootModule::Setup( QObject* obj, QDomElement config ) {
    _shell = qobject_cast<Shell*>( obj );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( "Root" );
    Q_ASSERT( _region != 0 );
    _ui = new MainUI( _shell, _region->widget() );
    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
}

bool RootModule::event( QEvent* event ) {
    return false;
}

Q_EXPORT_PLUGIN2( RootModule, RedBullPlayer::Modules::Root::RootModule )
