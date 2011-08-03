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
#ifndef TASKMANAGERMODULE_H
#define TASKMANAGERMODULE_H

//QT Includes
#include <QObject>
#include <QTimer>
#include <QHash>

// Project Includes
#include "../../Interfaces/TaskManager/ITaskManager.h"
#include "../../Interfaces/TaskManager/ITask.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/TaskManager/TaskExecuteRequestEvent.h"
#include "../../Events/TaskManager/TaskCancelRequestEvent.h"
#include "taskrepository.h"
#include "taskexecutor.h"
#include "taskrepositoryeventbridge.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskManagerModule : public ITaskManager {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::TaskManager::ITaskManager )

                public:
                    TaskManagerModule();
                    virtual ~TaskManagerModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "TaskManager";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                    virtual void cleanUp();

                protected:
                    bool event( QEvent * e );

                    void onTaskCancelRequest( RedBullPlayer::Events::TaskCancelRequestEvent* e );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    QTimer* _executionTimer;

                    TaskRepository* _repository;
                    QHash<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, TaskExecutor*>* _executorList;
                    TaskRepositoryEventBridge* _repositoryEventBridge;

                    void bootstrap();

                    TaskExecutor* createExecutor();
                    bool isExecutorRunning( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );
                    void tryExecute( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );


                private slots:
                    void onExecutionTimer();
                    void taskFinished( QUuid );
                    void taskCanceled( QUuid );
                    void taskRunning( QUuid );
            };
        }
    }
}

#endif // TASKMANAGERMODULE_H
