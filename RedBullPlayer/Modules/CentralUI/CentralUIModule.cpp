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
#include "CentralUIModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::CentralUI;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;

CentralUIModule::CentralUIModule() : ICentralUI() {
}

void CentralUIModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new CentralUICreator( _shell, _region->widget() );
    _region->widget()->layout()->addWidget( _ui );

    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( PlayerVisibilityChanged, this );
}

bool CentralUIModule::event ( QEvent * e ) {
    if( e->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusedChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == PlayerVisibilityChanged ) {
        onPlayerVisibilityChanged( static_cast<PlayerVisibilityChangedEvent*>( e ) );
    }

    return false;
}

void CentralUIModule::onPlayerVisibilityChanged( PlayerVisibilityChangedEvent *e ) {
    _ui->handlePlayerVisibilityChanged( e->visible() );
}

void CentralUIModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *e ) {
    _ui->changeMainUIRegionFocusGraphics( e->uiRegion() );
}

void CentralUIModule::onCentralFocusedChanged( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

void CentralUIModule::setCurrentWidget( RedBullPlayer::Events::CentralUIWidgetChangedEvent::CentralUIWidgets widget ) {
    _ui->setCurrentWidget( widget );
    CentralUIWidgetChangedEvent e( widget );
    Aggregator::instance()->sendEvent( &e );
}

void CentralUIModule::emitCentralFocusChangedEvent( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    CentralFocusChangedEvent e( widgetType );
    Aggregator::instance()->sendEvent( &e );
}

Q_EXPORT_PLUGIN2( CentralUIModule, RedBullPlayer::Modules::CentralUI::CentralUIModule )
