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
#ifndef STATEICONMODEL_H
#define STATEICONMODEL_H

#include <QObject>
#include "../../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../../Events/StateDisplay/StatusChangedEvent.h"
#include "../../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class StateIconModel : public QObject {
                    Q_OBJECT
                public:
                    StateIconModel( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose,
                                    RedBullPlayer::Events::HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition,
                                    QObject *parent = 0 );
                    virtual ~StateIconModel();

                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose() const {
                        return _purpose;
                    }
                    RedBullPlayer::Events::HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition() const {
                        return _hoverBoxPosition;
                    }

                protected:
                    bool event( QEvent* event );
                    void onStatusChangedEvent( RedBullPlayer::Events::StatusChangedEvent* e );

                private: //data
                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;
                    RedBullPlayer::Events::HoverBoxChangeVisibilityEvent::HoverBoxDockPosition _hoverBoxPosition;

                signals:
                    void buttonVisibilityChanged( bool visible );
            };
        }
    }
}

#endif // STATEICONMODEL_H
