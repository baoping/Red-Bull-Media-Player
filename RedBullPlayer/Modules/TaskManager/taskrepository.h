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
#ifndef TASKREPOSITORY_H
#define TASKREPOSITORY_H

#include <QObject>
#include <QHash>
#include <QQueue>
#include <QUuid>
#include <QPair>

#include "../../Interfaces/TaskManager/ITask.h"
#include "../../Interfaces/TaskManager/Types.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskRepository : public QObject {
                    Q_OBJECT

                public:
                    TaskRepository( QObject *parent = 0 );
                    virtual ~TaskRepository();

                    int count();
                    int count( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                    void enqueue( ITask* task );
                    ITask* next( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                    RedBullPlayer::Modules::TaskManager::TaskInfo get( QUuid taskId, bool* ok );
                    RedBullPlayer::Modules::TaskManager::TaskInfo get( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, bool* ok );

                    bool contains( QUuid taskId );
                    bool contains( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                    bool remove( QUuid taskId );
                    bool remove( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                protected:
                    bool event( QEvent* event );

                signals:
                    void taskAdded( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, TaskInfo taskInfo );
                    void taskRemoved( QUuid taskId );

                private:
                    void bootstrap();
                    QQueue<ITask*>* addQueueForPurpose( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );
                    ITask* findInQueue( QUuid taskId, QQueue<ITask*>* queue );
                    bool removeFromQueue( QUuid taskId, QQueue<ITask*>* queue );
                    ITask* getTask( QUuid taskId );
                    ITask* getTask( QUuid taskId, RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                    QHash<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, QQueue<ITask*>*>* _taskQueueCollection;
            };
        }
    }
}

#endif // TASKREPOSITORY_H
