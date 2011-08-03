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
#ifndef STATISTICCLIENTCACHEREPOSITORY_H
#define STATISTICCLIENTCACHEREPOSITORY_H


// Qt includes
#include <QObject>
#include <QString>
#include <QMutex>
#include <QUuid>

// Project includes
#include "MediaDB.h"
#include "../../Container/IError.h"
#include "../../Interfaces/MediaLibrary/IStatisticClientCacheRepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class StatisticClientCacheRepository :  public QObject, public IStatisticClientCacheRepository {
                    Q_OBJECT
                public:
                    StatisticClientCacheRepository( MediaDB* mediaDb, QObject *parent );
                    virtual ~StatisticClientCacheRepository();
                    virtual IStatisticEntry* createEntry( QString entryType, QString data );
                    virtual IStatisticEntry* createInMemoryEntry( QString entryType, QString data );
                    virtual bool deleteEntry( QUuid guid, RedBullPlayer::IError* error );
                    virtual QList<IStatisticEntry*>* getAllEntries();

                private:
                    StatisticClientCacheRepository() {};
                    MediaDB* _mediaDb;

                    mutable QMutex _createEntryMutex;
                    mutable QMutex _deleteEntryMutex;
            };
        }
    }
}
#endif // STATISTICCLIENTCACHEREPOSITORY_H
