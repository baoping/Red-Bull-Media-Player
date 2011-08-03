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
#ifndef STATEPRESENTEREVENTBRIDGE_H
#define STATEPRESENTEREVENTBRIDGE_H

#include <QObject>

#include "../../Interfaces/StateDisplay/IStatePresenter.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"
#include "../../Events/ShellWidgetMovedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class StatePresenterEventBridge : public QObject {
                    Q_OBJECT

                public:
                    StatePresenterEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, IStatePresenter* statePresenter, QObject *parent = 0 );
                protected:
                    bool event( QEvent* event );

                    void onHoverBoxChangeVisibility( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent* e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent *e );
                private:
                    IStatePresenter* _statePresenter;
                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;
            };
        }
    }
}


#endif // STATEPRESENTEREVENTBRIDGE_H
