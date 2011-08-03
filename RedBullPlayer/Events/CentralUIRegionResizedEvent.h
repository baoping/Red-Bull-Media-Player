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
#ifndef CENTRALUIREGIONRESIZEDEVENT_H
#define CENTRALUIREGIONRESIZEDEVENT_H

//QT Includes
#include <QEvent>
#include <QSize>

// Project Includes
#include "Events_global.h"
#include "EventTypes.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT CentralUIRegionResizedEvent : public QEvent {
            public:
                CentralUIRegionResizedEvent( int width, int height ) : QEvent( CentralUIRegionResized ) {
                    _width = width;
                    _height = height;
                }
                int width() const {
                    return _width;
                }
                int height() const {
                    return _height;
                }
            private:
                int _width;
                int _height;
        };
    }
}

#endif // CENTRALUIREGIONRESIZEDEVENT_H
