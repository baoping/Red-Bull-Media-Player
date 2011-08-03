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
#include "SeekBarEventBridge.h"

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/SeekBar/SeekBarPlayHeadChangedEvent.h"

using namespace RedBullPlayer::Modules::SeekBar;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

SeekBarEventBridge::SeekBarEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( UpdateSeekBarModel, this );
    Aggregator::instance()->registerListener( PlayerTimeChanged, this );
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
    Aggregator::instance()->registerListener( PlayerStateChanged, this );
    Aggregator::instance()->registerListener( PlayerTrackStarted, this );
    Aggregator::instance()->registerListener( BufferingAnimationChangeVisibility, this );
}

SeekBarEventBridge::~SeekBarEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool SeekBarEventBridge::event( QEvent* event ) {
    if ( event->type() == UpdateSeekBarModel ) {
        onUpdateSeekBarModel( static_cast<UpdateSeekBarModelEvent*>( event ) );
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

    if ( event->type() == PlayerTrackStarted ) {
        onPlayerTrackStarted( static_cast<PlayerTrackStartedEvent*>( event ) );
    }

    if ( event->type() == BufferingAnimationChangeVisibility ) {
        onBufferingAnimationChangeVisibility( static_cast<BufferingAnimationChangeVisibilityEvent*>( event ) );
    }

    return false;
}

void SeekBarEventBridge::onBufferingAnimationChangeVisibility( BufferingAnimationChangeVisibilityEvent* e ) {
    emit bufferingAnimationChangedVisibility( e->visible() );
}

void SeekBarEventBridge::onPlayerTimeChanged( PlayerTimeChangedEvent* e ) {
    emit playerTimeChanged( e->time() );
}

void SeekBarEventBridge::onUpdateSeekBarModel( UpdateSeekBarModelEvent* e ) {
    emit updateSeekBarModel( e->media() );
}

void SeekBarEventBridge::onPlayListItemSelected( PlaylistItemSelectedEvent* e ) {
    emit updateSeekBarModel( e->playlistItem()->getMedia() );
}

void SeekBarEventBridge::onPlayerStateChanged( PlayerStateChangedEvent* e ) {
    emit playerStateChanged( e->state() );
}

void SeekBarEventBridge::onPlayerTrackStarted( PlayerTrackStartedEvent* e ) {
    emit playerTrackStarted();
}

void SeekBarEventBridge::seekBarValueChanged( float value ) {
    SeekBarPlayHeadChangedEvent event( value );
    Aggregator::instance()->sendEvent( &event );
}

