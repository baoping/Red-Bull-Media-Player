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
#ifndef CANCELCOVERIMAGECREATIONEVENT_H
#define CANCELCOVERIMAGECREATIONEVENT_H

#include <QEvent>
#include <QString>
#include <QList>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"

namespace RedBullPlayer {
    namespace Events {

        class EVENTSSHARED_EXPORT CancelCoverImageCreationEvent : public QEvent {
            public:
                CancelCoverImageCreationEvent( QUuid mediaId );
                QUuid mediaId() const {
                    return _mediaId;
                }
            private:
                CancelCoverImageCreationEvent() : QEvent( CancelCoverImageCreation ) {}

                QUuid _mediaId;
        };
    }
}
#endif // CANCELCOVERIMAGECREATIONEVENT_H