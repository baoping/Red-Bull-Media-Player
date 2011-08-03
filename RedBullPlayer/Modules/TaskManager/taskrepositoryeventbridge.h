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
#ifndef TASKREPOSITORYEVENTBRIDGE_H
#define TASKREPOSITORYEVENTBRIDGE_H

#include <QObject>
#include <QHash>
#include <QQueue>
#include <QUuid>

#include "../../Interfaces/TaskManager/ITask.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/TaskManager/TaskExecuteRequestEvent.h"
#include "../../Events/TaskManager/TaskCancelRequestEvent.h"
#include "taskrepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskRepositoryEventBridge : public QObject {
                    Q_OBJECT

                public:
                    TaskRepositoryEventBridge( TaskRepository* repository, RedBullPlayer::EventAggregator::Aggregator* eventAggregator, QObject *parent = 0 );

                protected:
                    bool event( QEvent* event );
                    void onTaskExecuteRequest( RedBullPlayer::Events::TaskExecuteRequestEvent* e );
                    void onTaskCancelRequest( RedBullPlayer::Events::TaskCancelRequestEvent* e );

                private:
                    TaskRepository* _repository;
                    RedBullPlayer::EventAggregator::Aggregator* _eventAggregator;

                private slots:
                    void taskAdded( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, TaskInfo taskInfo );
                    void taskRemoved( QUuid taskId );
            };
        }
    }
}

#endif // TASKREPOSITORYEVENTBRIDGE_H
