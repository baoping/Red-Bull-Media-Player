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
#ifndef STATUSCHANGEDEVENT_H
#define STATUSCHANGEDEVENT_H
//QT Includes
#include <QEvent>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT StatusChangedEvent : public QEvent {
            public:
                enum GroupState {
                    ACTIVE,
                    INACTIVE
                };

                StatusChangedEvent( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum statusGroup, GroupState groupState ) : QEvent( StatusChanged ) {
                    _statusGroup = statusGroup;
                    _groupState = groupState;
                }

                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum statusGroup() {
                    return _statusGroup;
                }
                GroupState groupState() {
                    return _groupState;
                }

            private:
                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _statusGroup;
                GroupState _groupState;
        };
    }
}


#endif // STATUSCHANGEDEVENT_H
