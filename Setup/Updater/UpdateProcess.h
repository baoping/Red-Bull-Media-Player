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
#ifndef UPDATEPROCESS_H
#define UPDATEPROCESS_H

//QT Includes
#include <QObject>
#include <QString>
#include <QList>

// Project includes
#include "UpdateTask.h"
#include "../UpdateServices/PlayerVersion.h"

namespace RedBullPlayer {
    namespace Updater {
        class UpdateProcess  : public QObject {
                Q_OBJECT
            public:
                UpdateProcess( QString fromVersion, QString toVersion, QObject* parent );
                virtual ~UpdateProcess() {}

                const QList<UpdateTask*>& tasks() {
                    return _tasks;
                }

                void addTask( UpdateTask* );
                void execute();

                RedBullPlayer::Updater::PlayerVersion* fromVersion() const {
                    return _fromVersion;
                }

                RedBullPlayer::Updater::PlayerVersion* toVersion() const {
                    return _toVersion;
                }
            private:
                QList<UpdateTask*> _tasks;
                QString _name;
                RedBullPlayer::Updater::PlayerVersion* _fromVersion;
                RedBullPlayer::Updater::PlayerVersion* _toVersion;

            signals:
                void taskStarted( QString name );
                void taskFinished( QString name );
                void taskFailed( QString name, QString errorMsg );

                // Static
            public:
                static bool lessThen( UpdateProcess* first,  UpdateProcess* second );
        };
    }
}

#endif // UPDATEPROCESS_H
