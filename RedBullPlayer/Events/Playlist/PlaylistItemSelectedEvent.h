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
#ifndef PLAYLISTITEMSELECTEDEVENT_H
#define PLAYLISTITEMSELECTEDEVENT_H

//QT Includes
#include <QEvent>

// Project Includes
#include "../Events_global.h"
#include "../../Interfaces/IPlaylistItem.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT PlaylistItemSelectedEvent : public QEvent {
            public:
                PlaylistItemSelectedEvent( RedBullPlayer::Modules::Playlist::IPlaylistItem  *item,
                                           bool skipDocuments = false,
                                           bool fromSelectPreviousItemEvent = false,
                                           bool overrideFunctionSelectEvent = false );

                RedBullPlayer::Modules::Playlist::IPlaylistItem *playlistItem();
                bool skipDocuments();
                bool fromSelectPreviousItemEvent();
                bool overrideFunctionSelectedEvent();

            private:
                RedBullPlayer::Modules::Playlist::IPlaylistItem *_item;
                bool _skipDocuments;
                bool _fromSelectPreviousItemEvent;
                bool _overrideFunctionSelectEvent;
        };
    }
}

#endif // PLAYLISTITEMSELECTEDEVENT_H
