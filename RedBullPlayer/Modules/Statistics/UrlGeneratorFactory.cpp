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
#include "UrlGeneratorFactory.h"

#include "../../Container/Constants.h"

#include "PlayerStartedUrlGenerator.h"
#include "PodCastViewStatisticUrlGenerator.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

IUrlGenerator* UrlGeneratorFactory::getGeratorForRequest(
    const IStatisticEntry& entry ) {
    if( entry.entryType() == Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED ) {
        return new PlayerStartedUrlGenerator();
    }

    if( entry.entryType() == Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW ) {
        return new PodCastViewStatisticUrlGenerator();
    }

    return 0;
}
