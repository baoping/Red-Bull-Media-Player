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
#ifndef TASKPROGRESSEVENT_H
#define TASKPROGRESSEVENT_H

//QT Includes
#include <QEvent>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT TaskProgressEvent : public QEvent {
            public:
                TaskProgressEvent( QUuid taskId, int progress ) : QEvent( TaskProgress ) {
                    _taskId = taskId;
                    _progress = progress;
                }

                QUuid taskId() const {
                    return _taskId;
                }
                int progress() const {
                    return _progress;
                }

            private:
                QUuid _taskId;
                int _progress;
        };
    }
}
#endif // TASKPROGRESSEVENT_H