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
#include "InternetAvailabilityPresenterEventBridge.h"

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

InternetAvailabilityPresenterEventBridge::InternetAvailabilityPresenterEventBridge( TaskPurpose::PurposeEnum purpose,
        IInternetAvailabilityPresenter *internetAvailabilityPresenter,
        QObject *parent )
    : StatePresenterEventBridge( purpose, internetAvailabilityPresenter, parent ) {
    _internetAvailabilityPresenter = internetAvailabilityPresenter;
    _purpose = purpose;
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
}

bool InternetAvailabilityPresenterEventBridge::event( QEvent *event ) {
    if( event->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( event ) );
    }

    StatePresenterEventBridge::event( event );
    return false;
}

void InternetAvailabilityPresenterEventBridge::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent *event ) {
    _internetAvailabilityPresenter->handleInternetAvailabilityChanged( event->internetAvailable() );
}
