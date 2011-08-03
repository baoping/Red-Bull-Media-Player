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
#ifndef TASKREPOSITORYTASKADDEDEVENT_H
#define TASKREPOSITORYTASKADDEDEVENT_H

//QT Includes
#include <QEvent>
#include <QUuid>
#include <QPair>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"

#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../Interfaces/TaskManager/Types.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT TaskRepositoryTaskAddedEvent : public QEvent {
            public:
                TaskRepositoryTaskAddedEvent( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo ) : QEvent( TaskRepositoryTaskAdded ) {
                    _taskInfo = taskInfo;
                    _purpose = purpose;
                }

                RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo() const {
                    return _taskInfo;
                }
                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose() const {
                    return _purpose;
                }

            private:
                RedBullPlayer::Modules::TaskManager::TaskInfo _taskInfo;
                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;
        };
    }
}

#endif // TASKREPOSITORYTASKADDEDEVENT_H
