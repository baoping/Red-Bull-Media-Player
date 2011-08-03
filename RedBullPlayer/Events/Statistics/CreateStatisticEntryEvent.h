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
#ifndef CREATESTATISTICENTRYEVENT_H
#define CREATESTATISTICENTRYEVENT_H

//QT Includes
#include <QEvent>

#include "../Events_global.h"
#include "../EventTypes.h"


namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT CreateStatisticEntryEvent : public QEvent {
            public:
                CreateStatisticEntryEvent( QString eventType, QString data ) : QEvent( CreateStatisticsEntry ) {
                    _eventType = eventType;
                    _data = data;
                }
                ~CreateStatisticEntryEvent() {}
                QString eventType() const {
                    return _eventType;
                }
                QString data() const {
                    return _data;
                }
            private:
                CreateStatisticEntryEvent() : QEvent( CreateStatisticsEntry ) {}
                QString _eventType;
                QString _data;
        };
    }
}



#endif // CREATESTATISTICENTRYEVENT_H
