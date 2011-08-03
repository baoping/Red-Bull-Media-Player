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
#include "InternetAvailabilityModel.h"

#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;

InternetAvailabilityModel::InternetAvailabilityModel( HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition,
        IMediaLibrary *mediaLibrary, QObject *parent )
    : StateIconModel( TaskPurpose::INTERNET_AVAILABILITY, hoverBoxPosition, parent ) {
    _mediaLibrary = mediaLibrary;
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
}

bool InternetAvailabilityModel::event( QEvent *event ) {
    if( event->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( event ) );
    }

    return false;
}

void InternetAvailabilityModel::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent *event ) {
    emit internetConnectionStatusChanged( event->internetAvailable() );
}
