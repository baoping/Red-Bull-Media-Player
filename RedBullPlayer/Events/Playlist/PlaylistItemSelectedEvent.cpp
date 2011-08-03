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
#include "PlaylistItemSelectedEvent.h"

// Project Includes
#include "../EventTypes.h"

using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::Playlist;

PlaylistItemSelectedEvent::PlaylistItemSelectedEvent( IPlaylistItem  *item,
        bool skipDocuments,
        bool fromSelectPreviousItemEvent,
        bool overrideFunctionSelectEvent ) :
    QEvent( PlaylistItemSelected ) {
    _item = item;
    _skipDocuments = skipDocuments;
    _fromSelectPreviousItemEvent = fromSelectPreviousItemEvent;
    _overrideFunctionSelectEvent = overrideFunctionSelectEvent;
}

IPlaylistItem *PlaylistItemSelectedEvent::playlistItem() {
    return _item;
}

bool PlaylistItemSelectedEvent::skipDocuments() {
    return _skipDocuments;
}

bool PlaylistItemSelectedEvent::fromSelectPreviousItemEvent() {
    return _fromSelectPreviousItemEvent;
}

bool PlaylistItemSelectedEvent::overrideFunctionSelectedEvent() {
    return _overrideFunctionSelectEvent;
}
