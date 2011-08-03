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
#ifndef MEDIADISCOVEREDEVENT_H
#define MEDIADISCOVEREDEVENT_H

#include <QEvent>
#include <QString>
#include <QList>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "../../Container/Constants.h"


namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT MediaDiscoveredEvent : public QEvent {
            public:
                enum MediaDiscoveredSource {
                    POD_CATCHER = 0x01,
                    SEARCH = 0x02,
                    STARTUP_MEDIALIBRARY = 0x04,
                    STARTUP_FOLDER = 0x08,
                    USER_ADD = 0x16
                };

                MediaDiscoveredEvent( QUuid mediaId,
                                      MediaDiscoveredSource source
                                    ) : QEvent( MediaDiscovered ) {
                    _mediaId = mediaId;
                    _source = source;
                }

                QUuid mediaId() const {
                    return _mediaId;
                }

                MediaDiscoveredSource source() const {
                    return _source;
                }

            private:
                MediaDiscoveredEvent() : QEvent( MediaDiscovered ) {}
                QUuid _mediaId;
                MediaDiscoveredSource _source;
        };
    }
}
#endif // MEDIADISCOVEREDEVENT_H
