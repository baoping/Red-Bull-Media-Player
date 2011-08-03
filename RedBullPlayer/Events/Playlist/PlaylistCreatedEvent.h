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
#ifndef PLAYLISTCREATEDEVENT_H
#define PLAYLISTCREATEDEVENT_H

//QT Includes
#include <QEvent>
#include <QPointer>

// Project Includes
#include "../Events_global.h"
#include "../../Interfaces/IPlaylist.h"
#include "PlaylistContentTypeChangedEvent.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT  PlaylistCreatedEvent : public QEvent {
            public:
                PlaylistCreatedEvent( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist,
                                      RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType type );
                QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist();
                RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType contentType();

            private:
                QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _playlist;
                RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType _type;
        };
    }
}

#endif // PLAYLISTCREATEDEVENT_H
