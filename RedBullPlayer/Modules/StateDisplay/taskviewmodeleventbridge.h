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
#ifndef TASKVIEWMODELEVENTBRIDGE_H
#define TASKVIEWMODELEVENTBRIDGE_H

#include <QObject>

#include "../../Events/TaskManager/TaskRepositoryTaskAddedEvent.h"
#include "../../Events/TaskManager/TaskRepositoryTaskRemovedEvent.h"
#include "../../Events/TaskManager/TaskStartedEvent.h"
#include "../../Interfaces/StateDisplay/ITaskModel.h"
#include "../../Events/TaskManager/TaskProgressEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class TaskViewModelEventBridge : public QObject {
                    Q_OBJECT

                public:
                    TaskViewModelEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, ITaskModel* model, QObject *parent = 0 );
                protected:
                    bool event( QEvent* event );

                    //From Repository
                    void onTaskRepositoryTaskAdded( RedBullPlayer::Events::TaskRepositoryTaskAddedEvent* e );
                    void onTaskRepositoryTaskRemoved( RedBullPlayer::Events::TaskRepositoryTaskRemovedEvent* e );

                    //From Executor
                    void onTaskStarted( RedBullPlayer::Events::TaskStartedEvent* e );
                    void onTaskProgress( RedBullPlayer::Events::TaskProgressEvent* e );
                private:
                    ITaskModel* _model;
                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;

            };
        }
    }
}

#endif // TASKVIEWMODELEVENTBRIDGE_H
