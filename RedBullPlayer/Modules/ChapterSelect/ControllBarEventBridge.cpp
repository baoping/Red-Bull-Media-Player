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

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

ControllBarEventBridge::ControllBarEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( ControllBarPreviewButtonClicked, this );
    Aggregator::instance()->registerListener( ControllBarVisibilityChanged, this );
}

ControllBarEventBridge::~ControllBarEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool ControllBarEventBridge::event( QEvent* event ) {
    if ( event->type() == ControllBarPreviewButtonClicked ) {
        onControllBarPreviewButtonClicked( static_cast<ControllBarPreviewButtonClickedEvent*>( event ) );
    }

    if ( event->type() == ControllBarVisibilityChanged ) {
        onControllBarVisibilityChanged( static_cast<ControllBarVisibilityChangedEvent*>( event ) );
    }

    return false;
}

void ControllBarEventBridge::onControllBarPreviewButtonClicked( ControllBarPreviewButtonClickedEvent *e ) {
    emit previewClicked();
}

void ControllBarEventBridge::onControllBarVisibilityChanged( RedBullPlayer::Events::ControllBarVisibilityChangedEvent* e ) {
    emit controllBarVisibilityChanged( e->visible() );
}
