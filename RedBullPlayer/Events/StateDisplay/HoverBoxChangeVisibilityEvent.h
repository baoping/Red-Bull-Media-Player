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
#ifndef HOVERBOXCHANGEVISIBILITYEVENT_H
#define HOVERBOXCHANGEVISIBILITYEVENT_H

//QT Includes
#include <QEvent>
#include <QDebug>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "StatusChangedEvent.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT HoverBoxChangeVisibilityEvent : public QEvent {
            public:
                enum HoverBoxDockPosition {
                    UPPERLEFTCORNER,
                    LOWERLEFTCORNER,
                    UPPERRIGHTCORNER,
                    LOWERRIGHTCORNER
                };

                HoverBoxChangeVisibilityEvent( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum statusGroup, bool visible,
                                               int xGlobalPosition, int yGlobalPosition, HoverBoxDockPosition hoverBoxDockPosition ) : QEvent( HoverBoxChangeVisibility ) {
                    _statusGroup = statusGroup;
                    _visible = visible;
                    _xGlobalPosition = xGlobalPosition;
                    _yGlobalPosition = yGlobalPosition;
                    _hoverBoxDockPosition = hoverBoxDockPosition;
                }

                bool visible() {
                    return _visible;
                }
                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum statusGroup() {
                    return _statusGroup;
                }
                int xGlobalPosition() {
                    return _xGlobalPosition;
                }
                int yGlobalPosition() {
                    return _yGlobalPosition;
                }
                HoverBoxDockPosition hoverBoxDockPosition() {
                    return _hoverBoxDockPosition;
                }

            private:
                RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _statusGroup;
                bool _visible;
                int _xGlobalPosition;
                int _yGlobalPosition;
                HoverBoxDockPosition _hoverBoxDockPosition;
        };
    }
}

#endif // HOVERBOXCHANGEVISIBILITYEVENT_H
