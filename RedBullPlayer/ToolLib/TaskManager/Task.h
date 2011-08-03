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
#ifndef TASK_H
#define TASK_H

#include "../../Interfaces/TaskManager/ITask.h"



namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class Task : public ITask {
                    Q_OBJECT
                public:
                    Task( TaskPurpose::PurposeEnum purpose, QString name, QObject *parent = 0 );
                    virtual ~Task() {}

                    virtual QUuid guid() const {
                        return _guid;
                    }

                    virtual QString name() const {
                        return _name;
                    }

                    virtual void prepare();
                    virtual void execute();
                    virtual void cancel();
                    virtual void clean();

                    virtual TaskPurpose* purpose() {
                        return _purpose;
                    }

                    virtual TaskResult* result() {
                        return _result;
                    }

                    virtual TaskState* state() {
                        return _state;
                    }

                    virtual void setPurpose( TaskPurpose::PurposeEnum purpose );
                    virtual void setResult( TaskResult::ResultEnum result );
                    virtual void setState( TaskState::StateEnum state );

                    virtual TaskInfo taskInfo();

                protected:
                    bool isCanceled() const {
                        return _canceled;
                    }

                    void setCanceled( bool canceled ) {
                        _canceled = canceled;
                    }

                private:
                    QUuid _guid;
                    QString _name;
                    TaskPurpose* _purpose;
                    TaskResult* _result;
                    TaskState* _state;
                    bool _canceled;
            };
        }
    }
}

#endif // TASK_H
