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
#ifndef MEDIAVANISHEDEVENT_H
#define MEDIAVANISHEDEVENT_H
#include <QEvent>
#include <QString>
#include <QList>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "../../Container/Constants.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT MediaVanishedEvent : public QEvent {
            public:
                MediaVanishedEvent( QUuid mediaId ) : QEvent( MediaVanished ) {
                    _mediaId = mediaId;
                }

                QUuid mediaId() const {
                    return _mediaId;
                }

            private:
                MediaVanishedEvent() : QEvent( MediaVanished ) {}
                QUuid _mediaId;
        };
    }
}
#endif // MEDIAVANISHEDEVENT_H
