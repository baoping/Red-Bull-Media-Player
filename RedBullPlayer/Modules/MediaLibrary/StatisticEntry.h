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
#ifndef STATISTICENTRY_H
#define STATISTICENTRY_H

// Qt includes
#include <QString>
#include <QUuid>
#include <QDateTime>

// Project includes
#include "../../Interfaces/MediaLibrary/IStatisticEntry.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class StatisticEntry : public IStatisticEntry {
                public:
                    StatisticEntry( QUuid guid,
                                    QString entryType,
                                    QString data,
                                    QDateTime date );

                    virtual QUuid guid() const {
                        return _guid;
                    }

                    virtual QString entryType() const {
                        return _entryType;
                    }

                    virtual QString data() const {
                        return _data;
                    }

                    virtual QDateTime date() const {
                        return _date;
                    }

                private:
                    StatisticEntry();
                    QUuid _guid;
                    QString _entryType;
                    QString _data;
                    QDateTime _date;
            };
        }
    }
}

#endif // STATISTICENTRY_H
