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
#include "PlayerCloseButtonModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QLayout>

// Project Includes
#include "../../Widgets/CommandButton.h"
#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerCloseButtonClickedEvent.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Interfaces/Enumerations/MediaPlayerStateEnum.h"

using namespace RedBullPlayer::Modules::PlayerCloseButton;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Tools;

PlayerCloseButtonModule::PlayerCloseButtonModule() : IPlayerCloseButton() {
    _adjustPosOnNextTimeTick = false;
    _isFullScreen = true;
    DISTANCE_BETWEEN_BUTTONS = 10;
    _centralUIRegionWidth = 0;
    _centralUIRegionHeight = 0;
}

void PlayerCloseButtonModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new PlayerCloseButtonUI( _shell, _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    connect( _ui, SIGNAL( playerCloseButtonClicked() ), this, SLOT( exitClickedSlot() ) );
    initRegionWidget();
    _region->widget()->hide();
    Aggregator::instance()->registerListener( ShellWidgetMoved, this );
    Aggregator::instance()->registerListener( PlayerVisibilityChanged, this );
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    Aggregator::instance()->registerListener( ControllBarVisibilityChanged, this );
}

void PlayerCloseButtonModule::initRegionWidget() {
    QPixmap pixmap( Constants::PLAYLER_CLOSE_BUTTON_IMAGE_DIR + Constants::PLAYLER_CLOSE_BUTTON_BACK_2_PLAYLIST );
    QPixmap pixmap2( Constants::PLAYLER_CLOSE_BUTTON_IMAGE_DIR + Constants::PLAYLER_CLOSE_BUTTON_X_URL );
    _region->widget()->resize( pixmap2.width(), pixmap2.height() );
    QBitmap bmp( _region->widget()->size() );
    bmp.fill( Qt::color0 );
    QPainter painter( &bmp );
    painter.setPen( Qt::color1 );
    painter.setBrush( Qt::color1 );
    painter.drawRect( 0, 0, pixmap2.width(), pixmap2.height() );
    _region->widget()->setMask( bmp );
}

void PlayerCloseButtonModule::exitClickedSlot() {
    PlayerCloseButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void PlayerCloseButtonModule::back2PlaylistClickedSlot() {
}


bool PlayerCloseButtonModule::event ( QEvent * e ) {
    if ( e->type() == PlayerVisibilityChanged ) {
        onPlayerVisibilityChanged( static_cast<PlayerVisibilityChangedEvent*>( e ) );
    }

    if ( e->type() == CentralUIRegionResized  ) {
        onCentralUIRegionResized( static_cast<CentralUIRegionResizedEvent*>( e ) );
    }

    if ( e->type() == ShellWidgetMoved ) {
        onShellWidgetMoved( static_cast<ShellWidgetMovedEvent*>( e ) );
    }

    if ( e->type() == ControllBarVisibilityChanged ) {
        onControllBarVisibilityChanged( static_cast<ControllBarVisibilityChangedEvent*>( e ) );
    }

    return false;
}

void PlayerCloseButtonModule::onControllBarVisibilityChanged( ControllBarVisibilityChangedEvent *e ) {
    _isFullScreen = e->visible();

    if( e->visible() )
        _region->widget()->show();
    else
        _region->widget()->hide();
}

void PlayerCloseButtonModule::onPlayerVisibilityChanged( PlayerVisibilityChangedEvent* e ) {
    if( !e->visible() ) {
        _region->widget()->hide();
    } else {
        updatePosition();
    }
}

void PlayerCloseButtonModule::onCentralUIRegionResized( CentralUIRegionResizedEvent* e ) {
    _centralUIRegionWidth = e->width();
    _centralUIRegionHeight = e->height();
    updatePosition();
}

void PlayerCloseButtonModule::onShellWidgetMoved( ShellWidgetMovedEvent *e ) {
    updatePosition();
}


void PlayerCloseButtonModule::updatePosition() {
    QWidget *playerRegion = _shell->findRegion( "PlayerRegion" )->widget();

    if ( playerRegion ) {
        int x = playerRegion->mapToGlobal( QPoint( 0, 0 ) ).x() + _centralUIRegionWidth - _region->widget()->width() - Constants::PLAYLER_CLOSE_BUTTON_RIGHT;
        int y = playerRegion->mapToGlobal( QPoint( 0, 0 ) ).y() + Constants::PLAYLER_CLOSE_BUTTON_TOP;
        _region->widget()->move( x, y );

        if( playerRegion->isVisible() )
            _region->widget()->show();
    }
}

Q_EXPORT_PLUGIN2( PlayerCloseButtonModule, RedBullPlayer::Modules::PlayerCloseButton::PlayerCloseButtonModule )
