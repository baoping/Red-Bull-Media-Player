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
#ifndef TASKFINISHEDEVENT_H
#define TASKFINISHEDEVENT_H

//QT Includes
#include <QEvent>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT TaskFinishedEvent : public QEvent {
            public:
                TaskFinishedEvent( QUuid taskId ) : QEvent( TaskFinished ) {
                    _taskId = taskId;
                }

                QUuid taskId() const {
                    return _taskId;
                }

            private:
                QUuid _taskId;

        };
    }
}

#endif // TASKFINISHEDEVENT_H
