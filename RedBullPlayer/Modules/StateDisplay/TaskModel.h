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
#ifndef TASKMODEL_H
#define TASKMODEL_H

//QT Includes
#include <QObject>
#include <QHash>
#include <QPair>
#include <QUuid>

// Project Includes
#include "../../Interfaces/StateDisplay/ITaskModel.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class TaskModel : public ITaskModel {
                    Q_OBJECT

                public:
                    TaskModel( QObject * parent = 0 );
                    virtual ~TaskModel();

                    virtual int count() {
                        return _tasks->count();
                    }
                    virtual void add( RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo );
                    virtual RedBullPlayer::Modules::TaskManager::TaskInfo get( QUuid taskId, bool* ok );
                    virtual QList<RedBullPlayer::Modules::TaskManager::TaskInfo> all();
                    virtual void remove( QUuid taskId );
                    virtual void setStarted( QUuid taskId );
                    virtual void setProgress( QUuid taskId, int progress );

                private:
                    QUuid _guid;
                    QHash<QUuid, QString>* _tasks;
            };
        }
    }
}

#endif // TASKMODEL_H
