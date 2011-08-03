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
#ifndef TASKPURPOSEENUM_H
#define TASKPURPOSEENUM_H

#include <QMetaType>
#include <QObject>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class TaskPurpose : QObject {
                    Q_OBJECT
                    Q_ENUMS( PurposeEnum )

                public:
                    enum PurposeEnum {
                        UNKNOWN = 0x01,
                        DOWNLOAD = 0x02,
                        IO = 0x04,
                        UPDATE = 0x08,
                        DISK_SPACE = 0x16,
                        INTERNET_AVAILABILITY = 0x32
                    };

                    explicit TaskPurpose( PurposeEnum purpose ) : QObject( 0 ) {
                        _purpose = purpose;
                    }

                    PurposeEnum value() const {
                        return _purpose;
                    }

                    QString asString() {
                        return purposeString( _purpose );
                    }

                    bool isEqual( PurposeEnum purpose ) {
                        return _purpose == purpose;
                    }

                    static QString purposeString( PurposeEnum purpose ) {
                        QString purposeStr = "";

                        switch( purpose ) {
                            case TaskPurpose::UNKNOWN:
                                purposeStr = "UNKNOWN";
                                break;

                            case TaskPurpose::DOWNLOAD:
                                purposeStr = "DOWNLOAD";
                                break;

                            case TaskPurpose::IO:
                                purposeStr = "IO";
                                break;

                            case TaskPurpose::UPDATE:
                                purposeStr = "UPDATE";
                                break;

                            case TaskPurpose::DISK_SPACE:
                                purposeStr = "DISK_SPACE";
                                break;

                            case TaskPurpose::INTERNET_AVAILABILITY:
                                purposeStr = "INTERNET_AVAILABILITY";
                                break;
                        }

                        return purposeStr;
                    }
                private:
                    PurposeEnum _purpose;
            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum );


#endif // TASKPURPOSEENUM_H
