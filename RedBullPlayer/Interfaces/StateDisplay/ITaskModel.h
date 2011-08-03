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
#ifndef ISTATEMODEL_H
#define ISTATEMODEL_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QPair>

#include "../TaskManager/Types.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class ITaskModel : public QObject {
                    Q_OBJECT

                public:
                    ITaskModel( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~ITaskModel() {}

                    virtual int count() = 0;
                    virtual void add( RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo ) = 0;
                    virtual RedBullPlayer::Modules::TaskManager::TaskInfo get( QUuid taskId, bool* ok ) = 0;
                    virtual QList<RedBullPlayer::Modules::TaskManager::TaskInfo> all() = 0;
                    virtual void remove( QUuid taskId ) = 0;
                    virtual void setStarted( QUuid taskId ) = 0;
                    virtual void setProgress( QUuid taskId, int progress ) = 0;

                signals:
                    void taskAdded( QUuid taskId );
                    void taskRemoved( QUuid taskId );
                    void taskStarted( QUuid taskId );
                    void taskProgress( QUuid taskId, int progress );
            };
        }
    }
}

#endif // ISTATEMODEL_H
