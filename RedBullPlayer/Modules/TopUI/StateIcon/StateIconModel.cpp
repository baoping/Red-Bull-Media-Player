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
#include "StateIconModel.h"

#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

StateIconModel::StateIconModel( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose,
                                HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition,
                                QObject *parent ) : QObject( parent ) {
    _purpose = purpose;
    _hoverBoxPosition = hoverBoxPosition;
    Aggregator::instance()->registerListener( StatusChanged, this );
}

StateIconModel::~StateIconModel() {
    Aggregator::instance()->unregisterListener( StatusChanged, this );
}

bool StateIconModel::event( QEvent* event ) {
    if ( event->type() == StatusChanged ) {
        onStatusChangedEvent( static_cast<StatusChangedEvent*>( event ) );
    }

    return false;
}

void StateIconModel::onStatusChangedEvent( StatusChangedEvent* e ) {
    if ( e->statusGroup() == _purpose ) {
        switch( e->groupState() ) {
            case ( StatusChangedEvent::ACTIVE ):
                emit buttonVisibilityChanged( true );
                break;

            case ( StatusChangedEvent::INACTIVE ):
                emit buttonVisibilityChanged( false );
                break;

            default:
                break;
        }
    }
}
