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
#ifndef CENTRALFOCUSCHANGEDEVENT_H
#define CENTRALFOCUSCHANGEDEVENT_H

#include <QEvent>
#include <QString>

// Project Includes
#include "Events_global.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT CentralFocusChangedEvent : public QEvent {
            public:
                enum CentralWidgetType {
                    UNKOWN = 0,
                    COVERVIEW_PLAYLIST = 1,
                    LISTVIEW_PLAYLIST = 2,
                    GRIDVIEW_PLAYLIST = 3,
                    TV_PLAYLIST = 4,
                    NEWS_PLAYLIST = 5,
                    ABOUT_PLAYLIST = 6,
                    VIDEOPLAYER = 7,
                    NEWS_DETAIL = 8,
                    ABOUT_DETAIL = 9,
                    SETTINGS_PLAYLIST = 10,
                    SETTINGS_DETAIL = 11
                };
                CentralFocusChangedEvent( CentralWidgetType widgetType );
                CentralWidgetType widgetType();

                static QString widgetTypeString( CentralWidgetType centralWidgetType );

            private:
                CentralWidgetType _widgetType;
        };
    }
}


#endif // CENTRALFOCUSCHANGEDEVENT_H
