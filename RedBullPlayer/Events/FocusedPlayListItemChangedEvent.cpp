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
#include "FocusedPlayListItemChangedEvent.h"


// Project Includes
#include "EventTypes.h"

using namespace RedBullPlayer::Events;


FocusedPlayListItemChangedEvent::FocusedPlayListItemChangedEvent( QString newTitle , PlaylistType playlistType ,
        int focusedItemIndex, bool selectedByUser, QUuid mediaId )
    : QEvent( FocusedPlaylistItemChanged ) {
    _playlistType = playlistType;
    _newTitle = newTitle;
    _focusedItemIndex = focusedItemIndex;
    _selectedByUser = selectedByUser;
    _mediaId = mediaId;
}

FocusedPlayListItemChangedEvent::PlaylistType FocusedPlayListItemChangedEvent::playlistType() {
    return _playlistType;
}

int FocusedPlayListItemChangedEvent::focusedItemIndex() {
    return _focusedItemIndex;
}

QString FocusedPlayListItemChangedEvent::newTitle() {
    return _newTitle;
}

bool FocusedPlayListItemChangedEvent::selectedByUser() {
    return _selectedByUser;
}

QUuid FocusedPlayListItemChangedEvent::mediaId() {
    return _mediaId;
}
