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
#ifndef TASKVIEWFACTORY_H
#define TASKVIEWFACTORY_H

//QT Includes
#include <QObject>
#include <QList>
#include <QPair>
#include <QUuid>

// Project Includes
#include "../../Interfaces/StateDisplay/ITaskViewFactory.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class TaskViewFactory : public ITaskViewFactory {
                    Q_OBJECT
                public:
                    TaskViewFactory( QObject* parent = 0 );
                    virtual QUuid guid() const {
                        return _guid;
                    }

                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle );
                    virtual ITaskView* createViewFor( RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo, QWidget* parent = 0 );

                private:
                    QUuid _guid;
                    RedBullPlayer::Modules::StyleFactory::ITextStyle* _textStyle;
            };
        }
    }
}

#endif // TASKVIEWFACTORY_H
