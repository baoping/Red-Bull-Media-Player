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
#ifndef TASKPRESENTEREVENTBRIDGE_H
#define TASKPRESENTEREVENTBRIDGE_H

#include <QObject>

#include "../../Interfaces/StateDisplay/ITaskPresenter.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"
#include "../../Events/ShellWidgetMovedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class TaskPresenterEventBridge : public QObject {
                    Q_OBJECT

                public:
                    TaskPresenterEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, ITaskPresenter* taskPresenter, QObject *parent = 0 );
                protected:
                    bool event( QEvent* event );

                    void onHoverBoxChangeVisibility( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent* e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent *e );
                private:
                    ITaskPresenter* _taskPresenter;
                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;
                    bool _lastState;

                private slots:
                    void stateIconVisibilityChanged( bool visible );
                    void requestTaskCancel( QUuid );
            };
        }
    }
}

#endif // TASKPRESENTEREVENTBRIDGE_H
