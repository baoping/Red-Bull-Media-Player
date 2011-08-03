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
#ifndef TASKSTATEENUM_H
#define TASKSTATEENUM_H

#include <QMetaType>
#include <QObject>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskState : QObject {
                    Q_OBJECT
                    Q_ENUMS( StateEnum )

                public:
                    enum StateEnum {
                        NEW = 0x01,
                        PENDING = 0x02,
                        RUNNING = 0x04,
                        CANCELED = 0x08,
                        EXECUTED = 0x16,
                    };

                    explicit TaskState( StateEnum state ) : QObject( 0 ) {
                        _state = state;
                    }

                    StateEnum value() const {
                        return _state;
                    }

                    QString asString() {
                        return stateString( _state );
                    }

                    bool isEqual( StateEnum state ) {
                        return _state == state;
                    }

                    static QString stateString( StateEnum state ) {
                        QString stateString = "";

                        switch( state ) {
                            case TaskState::NEW:
                                stateString = "NEW";
                                break;

                            case TaskState::PENDING:
                                stateString = "PENDING";
                                break;

                            case TaskState::RUNNING:
                                stateString = "RUNNING";
                                break;

                            case TaskState::CANCELED:
                                stateString = "CANCELED";
                                break;

                            case TaskState::EXECUTED:
                                stateString = "EXECUTED";
                                break;
                        }

                        return stateString;
                    }
                private:
                    StateEnum _state;

            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::TaskManager::TaskState::StateEnum );


#endif // TASKSTATEENUM_H
