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
#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

// QT includes
#include <QObject>

// Project includes
#include "UpdateProcess.h"
#include "../UpdateServices/PlayerVersion.h"

// QT includes
#include <QObject>
#include <QList>

// Project includes
#include "UpdateProcess.h"


namespace RedBullPlayer {
    namespace Updater {

        class UpdateManager : public QObject {
                Q_OBJECT
            public:
                UpdateManager( QObject* parent );

                void addProcess( UpdateProcess* prcess );

                void execute( PlayerVersion* fromVersion, PlayerVersion* toVersion );

                UpdateProcess* findMatchingProcess( PlayerVersion* fromVersion, PlayerVersion* toVersion );

                const QList<UpdateProcess*>& prcesses() const {
                    return _prcesses;
                }
            signals:
                void taskStarted( QString name );
                void taskFinished( QString name );
                void taskFailed( QString name, QString errorMsg );

            private:
                QList<UpdateProcess*> _prcesses;

        };

    }
}
#endif // UPDATEMANAGER_H
