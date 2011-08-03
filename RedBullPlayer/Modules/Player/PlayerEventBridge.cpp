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
#include "PlayerEventBridge.h"

#include <QDebug>
#include <QList>

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerVisibilityChangedEvent.h"
#include "../../Events/Player/PlayerPositionChangedEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/DocumentOpenedEvent.h"
#include "../../Events/Player/VideoScreenMouseMovedEvent.h"
#include "../../Events/Player/FullscreenStatusChangedEvent.h"
#include "../../Events/Player/PlayerForwardSpeedChangedEvent.h"
#include "../../Events/Player/PlayerBackwardSpeedChangedEvent.h"
#include "../../Events/Player/PlayerTrackStartedEvent.h"
#include "../../Events/Player/BufferingAnimationChangeVisibilityEvent.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

PlayerEventBridge::PlayerEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( PlayerRequestTimeChange, this );
    Aggregator::instance()->registerListener( PlayerVolumeChanged, this );
    Aggregator::instance()->registerListener( PlayerCloseButtonClicked, this );
    Aggregator::instance()->registerListener( PlayerExitClicked, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( FunctionSelected, this );
}

PlayerEventBridge::~PlayerEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool PlayerEventBridge::event( QEvent* event ) {
    if ( event->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( event ) );
    }

    if ( event->type() == PlayerRequestTimeChange ) {
        onPlayerRequestTimeChange( static_cast<PlayerRequestTimeChangeEvent*>( event ) );
    }

    if ( event->type() == PlayerVolumeChanged ) {
        onPlayerVolumeChangedEvent( static_cast<PlayerVolumeChangedEvent*>( event ) );
    }

    if ( event->type() == PlayerCloseButtonClicked ) {
        onPlayerCloseButtonClicked( static_cast<PlayerCloseButtonClickedEvent*>( event ) );
    }

    if ( event->type() == PlayerExitClicked ) {
        onPlayerExitClicked( static_cast<PlayerExitClickedEvent*>( event ) );
    }

    if ( event->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( event ) );
    }

    if ( event->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( event ) );
    }

    if ( event->type() == FunctionSelected ) {
        onFunctionSelected( static_cast<FunctionSelectedEvent*>( event ) );
    }

    return false;
}

void PlayerEventBridge::onPlayerVolumeChangedEvent( PlayerVolumeChangedEvent* e ) {
    emit playerVolumeChanged( e->volume() );
}

void PlayerEventBridge::onCentralFocusChanged( CentralFocusChangedEvent* e ) {
    emit centralFocusChanged( e->widgetType() );
}

void PlayerEventBridge::onPlayerRequestTimeChange( PlayerRequestTimeChangeEvent* e ) {
    emit requestTimeChange( e->time() );
}

void PlayerEventBridge::changePlayerVisibility( bool visible ) {
    PlayerVisibilityChangedEvent e( visible );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::changeCentralFocus( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType centralWidgetType ) {
    CentralFocusChangedEvent e( centralWidgetType );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::playerPositionChanged( float position ) {
    PlayerPositionChangedEvent e( position );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::playerTimeChanged( qint64 time ) {
    PlayerTimeChangedEvent e( time );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::mouseMovedOnPlayerView() {
    VideoScreenMouseMovedEvent videoScreenEvent;
    Aggregator::instance()->sendEvent( &videoScreenEvent );
    /* Setting Focus here is a hack because VLC subwindow swallow all mouse movements */
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::fullScreenChanged( bool isFullScreen ) {
    FullscreenStatusChangedEvent event( isFullScreen );
    Aggregator::instance()->sendEvent( &event );
}

void PlayerEventBridge::playerStateChanged( MediaPlayerState::StateEnum newState ) {
    PlayerStateChangedEvent e( newState );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::forwardSpeedChanged( int forwardSpeed ) {
    PlayerForwardSpeedChangedEvent e( forwardSpeed );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::backwardSpeedChanged( int backwardSpeed ) {
    PlayerBackwardSpeedChangedEvent e( backwardSpeed );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::playerTrackStarted( QUuid mediaId, QUuid mediaTypeId ) {
    PlayerTrackStartedEvent e( mediaId, mediaTypeId );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::changeBufferingAnimationVisibility( bool visible ) {
    BufferingAnimationChangeVisibilityEvent e( visible );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::documentOpened( QUuid mediaId ) {
    DocumentOpenedEvent e( mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void PlayerEventBridge::onPlayerCloseButtonClicked( PlayerCloseButtonClickedEvent* e ) {
    emit playerCloseButtonClicked();
}

void PlayerEventBridge::onPlayerExitClicked( PlayerExitClickedEvent* e ) {
    emit playerExitClicked();
}

void PlayerEventBridge::onKeyPressed( KeyPressedEvent* e ) {
    emit keyPressed( e->keyEvent() );
}

void PlayerEventBridge::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent* e ) {
    emit mainUIRegionFocusChanged( e->uiRegion() );
}

void PlayerEventBridge::onFunctionSelected( FunctionSelectedEvent* e ) {
    emit functionSelected( e->functionTitle() );
}




