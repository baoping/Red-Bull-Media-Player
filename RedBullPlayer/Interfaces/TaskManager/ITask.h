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
#ifndef ITASK_H
#define ITASK_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QMetaType>
#include <QPair>

#include "Types.h"

#include "../Enumerations/TaskPurposeEnum.h"
#include "../Enumerations/TaskResultEnum.h"
#include "../Enumerations/TaskStateEnum.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class ITask : public QObject  {
                    Q_OBJECT

                public:
                    ITask( QObject* parent = 0 ) : QObject( parent ) { }

                    virtual ~ITask() {}
                    virtual QUuid guid() const = 0;
                    virtual QString name() const = 0;
                    virtual void prepare() = 0;
                    virtual void execute() = 0;
                    virtual void cancel() = 0;
                    virtual void clean() = 0;

                    virtual RedBullPlayer::Modules::TaskManager::TaskPurpose* purpose() = 0;
                    virtual void setPurpose( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) = 0;

                    virtual RedBullPlayer::Modules::TaskManager::TaskResult* result() = 0;
                    virtual void setResult( RedBullPlayer::Modules::TaskManager::TaskResult::ResultEnum result ) = 0;

                    virtual RedBullPlayer::Modules::TaskManager::TaskState* state() = 0;
                    virtual void setState( RedBullPlayer::Modules::TaskManager::TaskState::StateEnum state ) = 0;

                    virtual TaskInfo taskInfo() = 0;

                signals:
                    void prepared( bool success );
                    void finished( bool success );
                    void canceled();
            };
        }
    }
}

#endif // ITASK_H
