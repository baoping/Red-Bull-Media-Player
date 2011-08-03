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
#ifndef PLAYLISTCONTENTTYPECHANGEDEVENT_H
#define PLAYLISTCONTENTTYPECHANGEDEVENT_H

//QT Includes
#include <QEvent>

// Project Includes
#include "../Events_global.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT PlaylistContentTypeChangedEvent : public QEvent {
            public:
                enum ContentType {
                    MEDIA,
                    TV,
                    NEWS,
                    ABOUT,
                    SETTINGS,
                    NONE
                };

                PlaylistContentTypeChangedEvent( ContentType type );
                ContentType newType();

            private:
                ContentType _type;
        };
    }
}

#endif // PLAYLISTCONTENTTYPECHANGEDEVENT_H
