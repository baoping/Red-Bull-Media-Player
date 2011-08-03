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
#include "BottomUIModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::BottomUI;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

BottomUIModule::BottomUIModule() : IBottomUI() {
}

void BottomUIModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new BottomUICreator( _shell, _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    connect( _ui, SIGNAL( dragging( QPoint ) ), this, SLOT( draggingSlot( QPoint ) ) );
    connect( _ui, SIGNAL( resizeWidgetClicked() ), this, SLOT( resizeClickSlot() ) );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
}

bool BottomUIModule::event ( QEvent * e ) {
    if( e->type() ==  MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    return false;
}

void BottomUIModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_BOTTOMUI, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_BOTTOMUI, QString( "Region has Main Focus = %1" ).arg( event->uiRegion() ) );
    _ui->changeMainUIRegionFocusGraphics( event->uiRegion() );
}

void BottomUIModule::draggingSlot( QPoint distance ) {
    QSize sizeIncrement( distance.x(), distance.y() );
    _shell->resize( _shell->size() + sizeIncrement );
}

void BottomUIModule::resizeClickSlot() {
    _shell->switchToNormalScreenForResize();
}

Q_EXPORT_PLUGIN2( BottomUIModule, RedBullPlayer::Modules::BottomUI::BottomUIModule )
