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
#include "ControllBarEventBridge.h"

#include <QDebug>

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/ControllBar/ControllBarVisibilityChangedEvent.h"
#include "../../Events/ControllBar/ControllBarBackwardButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarForwardButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarFullscreenButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPauseButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPlayButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPreviewButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarSnapshotButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarMuteButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarResolutionButtonClickedEvent.h"

using namespace RedBullPlayer::Modules::ControllBar;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

ControllBarEventBridge::ControllBarEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( UpdateControllBarModel, this );
    Aggregator::instance()->registerListener( PlayerTimeChanged, this );
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
    Aggregator::instance()->registerListener( PlayerStateChanged, this );
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    Aggregator::instance()->registerListener( ShellWidgetMoved, this );
    Aggregator::instance()->registerListener( PlayerVisibilityChanged, this );
    Aggregator::instance()->registerListener( VideoScreenMouseMoved, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( BufferingAnimationChangeVisibility, this );
    Aggregator::instance()->registerListener( SnapshotCaptureEnabled, this );
    Aggregator::instance()->registerListener( FullscreenStatusChanged, this );
    Aggregator::instance()->registerListener( PlayerForwardSpeedChanged, this );
    Aggregator::instance()->registerListener( PlayerBackwardSpeedChanged, this );
    Aggregator::instance()->registerListener( PlayerVolumeChanged, this );
    Aggregator::instance()->registerListener( PlayerExitClicked, this );
}

ControllBarEventBridge::~ControllBarEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool ControllBarEventBridge::event( QEvent* event ) {
    if ( event->type() == UpdateControllBarModel ) {
        onUpdateControllBarModel( static_cast<UpdateControllBarModelEvent*>( event ) );
    }

    if ( event->type() == PlayerTimeChanged ) {
        onPlayerTimeChanged( static_cast<PlayerTimeChangedEvent*>( event ) );
    }

    if ( event->type() == PlaylistItemSelected ) {
        onPlayListItemSelected( static_cast<PlaylistItemSelectedEvent*>( event ) );
    }

    if ( event->type() == PlayerStateChanged ) {
        onPlayerStateChanged( static_cast<PlayerStateChangedEvent*>( event ) );
    }

    if ( event->type() == CentralUIRegionResized )  {
        onCentralUIRegionResized( static_cast<CentralUIRegionResizedEvent*>( event ) );
    }

    if ( event->type() == PlayerVisibilityChanged ) {
        onPlayerVisibilityChanged( static_cast<PlayerVisibilityChangedEvent*>( event ) );
    }

    if ( event->type() == VideoScreenMouseMoved ) {
        onVideoScreenMouseMoved( static_cast<VideoScreenMouseMovedEvent*>( event ) );
    }

    if ( event->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( event ) );
    }

    if ( event->type() == BufferingAnimationChangeVisibility ) {
        onBufferingAnimationChangeVisibility( static_cast<BufferingAnimationChangeVisibilityEvent*>( event ) );
    }

    if ( event->type() == SnapshotCaptureEnabled ) {
        onSnapshotCaptureEnabled( static_cast<SnapshotCaptureEnabledEvent*>( event ) );
    }

    if ( event->type() == FullscreenStatusChanged ) {
        onFullscreenStatusChanged( static_cast<FullscreenStatusChangedEvent*>( event ) );
    }

    if ( event->type() == PlayerForwardSpeedChanged ) {
        onPlayerForwardSpeedChanged( static_cast<PlayerForwardSpeedChangedEvent*>( event ) );
    }

    if ( event->type() == PlayerBackwardSpeedChanged ) {
        onPlayerBackwardSpeedChanged( static_cast<PlayerBackwardSpeedChangedEvent*>( event ) );
    }

    if ( event->type() == PlayerVolumeChanged ) {
        onPlayerVolumneChanged( static_cast<PlayerVolumeChangedEvent*>( event ) );
    }

    if ( event->type() == ShellWidgetMoved ) {
        onShellWidgetMoved( static_cast<ShellWidgetMovedEvent*>( event ) );
    }

    if ( event->type() == PlayerExitClicked ) {
        onPlayerExitClicked( static_cast<PlayerExitClickedEvent*>( event ) );
    }

    return false;
}

void ControllBarEventBridge::onPlayerForwardSpeedChanged( PlayerForwardSpeedChangedEvent* e ) {
    emit playerForwardSpeedChanged( (int) e->speed() );
}

void ControllBarEventBridge::onPlayerBackwardSpeedChanged( PlayerBackwardSpeedChangedEvent* e ) {
    emit playerBackwardSpeedChanged( (int) e->speed() );
}

void ControllBarEventBridge::onPlayerVolumneChanged( PlayerVolumeChangedEvent* e ) {
    emit playerVolumeChanged( e->volume() );
}

void ControllBarEventBridge::onFullscreenStatusChanged( FullscreenStatusChangedEvent* e ) {
    emit fullScreenStatusChanged( e->isFullScreen() );
}

void ControllBarEventBridge::onSnapshotCaptureEnabled( SnapshotCaptureEnabledEvent* e ) {
    emit snapshotCaptureEnabled( e->enabled() );
}

void ControllBarEventBridge::onPlayerTimeChanged( PlayerTimeChangedEvent* e ) {
    emit playerTimeChanged( e->time() );
}

void ControllBarEventBridge::onUpdateControllBarModel( UpdateControllBarModelEvent* e ) {
    emit updateControllBarModel( e->media() , false );
}

void ControllBarEventBridge::onPlayListItemSelected( PlaylistItemSelectedEvent* e ) {
    emit updateControllBarModel( e->playlistItem()->getMedia() , e->skipDocuments() );
}

void ControllBarEventBridge::onPlayerStateChanged( PlayerStateChangedEvent* e ) {
    emit playerStateChanged( e->state() );
}

void ControllBarEventBridge::onCentralUIRegionResized( CentralUIRegionResizedEvent* e ) {
    emit centralUIRegionResized( e->width(), e->height() );
}

void ControllBarEventBridge::onShellWidgetMoved( ShellWidgetMovedEvent* e ) {
    emit shellWidgetMoved();
}

void ControllBarEventBridge::onPlayerVisibilityChanged( PlayerVisibilityChangedEvent* e ) {
    emit playerVisibilityChanged( e->visible() );
}

void ControllBarEventBridge::onVideoScreenMouseMoved( VideoScreenMouseMovedEvent* e ) {
    emit videoScreenMouseMoved();
}

void ControllBarEventBridge::controllBarVisibiltyChanged( bool visible ) {
    ControllBarVisibilityChangedEvent e( visible );
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::onBufferingAnimationChangeVisibility( BufferingAnimationChangeVisibilityEvent* e ) {
    emit bufferingAnimationChangedVisibility( e->visible() );
}

void ControllBarEventBridge::onKeyPressed( KeyPressedEvent* e ) {
    emit keyPressed( e->keyEvent() );
}

void ControllBarEventBridge::onPlayerExitClicked( RedBullPlayer::Events::PlayerExitClickedEvent* e ) {
    emit playerExitClicked();
}


void ControllBarEventBridge::playClicked() {
    ControllBarPlayButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::pauseClicked() {
    ControllBarPauseButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::backwardClicked() {
    ControllBarBackwardButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::forwardClicked() {
    ControllBarForwardButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::previewClicked() {
    ControllBarPreviewButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::fullScreenClicked() {
    ControllBarFullscreenButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::snapshotClicked() {
    ControllBarSnapshotButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::muteClicked( bool mute ) {
    ControllBarMuteButtonClickedEvent e( mute );
    Aggregator::instance()->sendEvent( &e );
}

void ControllBarEventBridge::resolutionClicked() {
    ControllBarResolutionButtonClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
}
