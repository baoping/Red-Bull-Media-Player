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
#include "../../Events/SeekBar/UpdateSeekBarModelEvent.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;

SeekBarEventBridge::SeekBarEventBridge( QObject *parent ) : QObject( parent ) {
    Aggregator::instance()->registerListener( SeekBarPlayHeadChanged, this );
}

SeekBarEventBridge::~SeekBarEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool SeekBarEventBridge::event( QEvent* e ) {
    if ( e->type() == SeekBarPlayHeadChanged ) {
        onSeekBarPlayHeadChanged( static_cast<SeekBarPlayHeadChangedEvent*>( e ) );
    }

    return false;
}

void SeekBarEventBridge::updateSeekBarModel( IMedia* media ) {
    UpdateSeekBarModelEvent e( media );
    Aggregator::instance()->sendEvent( &e );
}

void SeekBarEventBridge::onSeekBarPlayHeadChanged( SeekBarPlayHeadChangedEvent* e ) {
    emit seekBarPlayerHeadChanged( e->position() );
}
