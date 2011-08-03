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
#include "StatisticEventBridge.h"

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Statistics/PodCastViewStatisticEntryEvent.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

StatisticEventBridge::StatisticEventBridge( QObject *parent ) : QObject( parent ) {
}

StatisticEventBridge::~StatisticEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

void StatisticEventBridge::createPodCastViewStatisticEntry( QString podCastId ) {
    PodCastViewStatisticEntryEvent e( podCastId );
    Aggregator::instance()->sendEvent( &e );
}
