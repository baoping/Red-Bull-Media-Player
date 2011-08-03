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

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/ControllBar/UpdateControllBarModelEvent.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;

ControllBarEventBridge::ControllBarEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( ControllBarPauseButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarPlayButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarSnapshotButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarFullscreenButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarMuteButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarForwardButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarBackwardButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarResolutionButtonClicked, this );
}

ControllBarEventBridge::~ControllBarEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool ControllBarEventBridge::event( QEvent* event ) {
    if ( event->type() == ControllBarPauseButtonClicked ) {
        onControllBarPauseButtonClicked( static_cast<ControllBarPauseButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarPlayButtonClicked ) {
        onControllBarPlayButtonClicked( static_cast<ControllBarPlayButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarSnapshotButtonClicked ) {
        onControllBarSnapshotButtonClicked( static_cast<ControllBarSnapshotButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarFullscreenButtonClicked ) {
        onControllBarFullscreenButtonClicked( static_cast<ControllBarFullscreenButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarMuteButtonClicked ) {
        onControllBarMuteButtonClicked( static_cast<ControllBarMuteButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarForwardButtonClicked ) {
        onControllBarForwardButtonClicked( static_cast<ControllBarForwardButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarBackwardButtonClicked ) {
        onControllBarBackwardButtonClicked( static_cast<ControllBarBackwardButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarResolutionButtonClicked ) {
        onControllBarResolutionButtonClicked( static_cast<ControllBarResolutionButtonClickedEvent*>( event ) );
    }

    return false;
}

void ControllBarEventBridge::onControllBarPauseButtonClicked( ControllBarPauseButtonClickedEvent *e ) {
    emit pauseClicked();
}

void ControllBarEventBridge::onControllBarPlayButtonClicked( ControllBarPlayButtonClickedEvent *e ) {
    emit playClicked();
}

void ControllBarEventBridge::onControllBarSnapshotButtonClicked( ControllBarSnapshotButtonClickedEvent *e ) {
    emit snapShotClicked();
}

void ControllBarEventBridge::onControllBarFullscreenButtonClicked( ControllBarFullscreenButtonClickedEvent *e ) {
    emit fullScreenClicked();
}

void ControllBarEventBridge::onControllBarMuteButtonClicked( ControllBarMuteButtonClickedEvent *e ) {
    emit muteClicked( e->muted() );
}

void ControllBarEventBridge::onControllBarForwardButtonClicked( ControllBarForwardButtonClickedEvent *e ) {
    emit forwardClicked();
}

void ControllBarEventBridge::onControllBarBackwardButtonClicked( ControllBarBackwardButtonClickedEvent *e ) {
    emit backwardClicked();
}

void ControllBarEventBridge::onControllBarResolutionButtonClicked( ControllBarResolutionButtonClickedEvent *e ) {
    emit resolutionClicked();
}

void ControllBarEventBridge::updateControllBarModel( IMedia* media ) {
    UpdateControllBarModelEvent e( media );
    Aggregator::instance()->sendEvent( &e );
}
