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
#ifndef FOCUSEDPLAYLISTITEMCHANGEDEVENT_H
#define FOCUSEDPLAYLISTITEMCHANGEDEVENT_H

//QT Includes
#include <QEvent>
#include <QSize>
#include <QString>
#include <QUuid>

// Project Includes
#include "Events_global.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT FocusedPlayListItemChangedEvent : public QEvent {
            public:
                enum PlaylistType {
                    COVERVIEW,
                    GRIDVIEW,
                    LISTVIEW,
                    TV,
                    NEWS,
                    ABOUT,
                    NONE
                };

                FocusedPlayListItemChangedEvent( QString newTitle, PlaylistType playlistType, int itemIndex, bool selectedByUser, QUuid mediaId );
                PlaylistType playlistType();
                QString newTitle();
                int focusedItemIndex();
                bool selectedByUser();
                QUuid mediaId();

            private:
                PlaylistType _playlistType;
                QString _newTitle;
                int _focusedItemIndex;
                bool _selectedByUser;
                QUuid _mediaId;
        };
    }
}

#endif // FOCUSEDPLAYLISTITEMCHANGEDEVENT_H
