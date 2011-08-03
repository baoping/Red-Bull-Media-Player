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
#ifndef TASKRESULTENUM_H
#define TASKRESULTENUM_H

#include <QMetaType>
#include <QObject>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskResult : QObject {
                    Q_OBJECT
                    Q_ENUMS( ResultEnum )

                public:
                    enum ResultEnum {
                        NOT_EXECUTED = 0x01,
                        SUCCESSFUL = 0x02,
                        FAILED = 0x04,
                    };

                    explicit TaskResult( ResultEnum result ) : QObject( 0 ) {
                        _result = result;
                    }

                    ResultEnum value() const {
                        return _result;
                    }

                    QString asString() {
                        return resultString( _result );
                    }

                    bool isEqual( ResultEnum result ) {
                        return _result == result;
                    }

                    static QString resultString( ResultEnum result ) {
                        QString resultStr = "";

                        switch( result ) {
                            case TaskResult::NOT_EXECUTED:
                                resultStr = "NOT_EXECUTED";
                                break;

                            case TaskResult::SUCCESSFUL:
                                resultStr = "SUCCESSFUL";
                                break;

                            case TaskResult::FAILED:
                                resultStr = "FAILED";
                                break;
                        }

                        return resultStr;
                    }
                private:
                    ResultEnum _result;

            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::TaskManager::TaskResult::ResultEnum );


#endif // TASKRESULTENUM_H
