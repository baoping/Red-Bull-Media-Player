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
#include "CentralFocusChangedEvent.h"

// Project Includes
#include "EventTypes.h"

using namespace RedBullPlayer::Events;

CentralFocusChangedEvent::CentralFocusChangedEvent( CentralWidgetType widgetType ): QEvent( CentralFocusChanged ) {
    _widgetType = widgetType;
}

CentralFocusChangedEvent::CentralWidgetType CentralFocusChangedEvent::widgetType() {
    return _widgetType;
}

QString CentralFocusChangedEvent::widgetTypeString( CentralWidgetType centralWidgetType ) {
    switch( centralWidgetType ) {
        case UNKOWN:
            return QString( "UNKOWN" );

        case COVERVIEW_PLAYLIST:
            return QString( "COVERVIEW_PLAYLIST" );

        case LISTVIEW_PLAYLIST:
            return QString( "LISTVIEW_PLAYLIST" );

        case GRIDVIEW_PLAYLIST:
            return QString( "GRIDVIEW_PLAYLIST" );

        case TV_PLAYLIST:
            return QString( "TV_PLAYLIST" );

        case NEWS_PLAYLIST:
            return QString( "NEWS_PLAYLIST" );

        case ABOUT_PLAYLIST:
            return QString( "ABOUT_PLAYLIST" );

        case VIDEOPLAYER:
            return QString( "VIDEOPLAYER" );

        case NEWS_DETAIL:
            return QString( "NEWS_DETAIL" );

        case ABOUT_DETAIL:
            return QString( "ABOUT_DETAIL" );

        case SETTINGS_PLAYLIST:
            return QString( "SETTINGS_PLAYLIST" );

        case SETTINGS_DETAIL:
            return QString( "SETTINGS_DETAIL" );
    }

    return "";
}
