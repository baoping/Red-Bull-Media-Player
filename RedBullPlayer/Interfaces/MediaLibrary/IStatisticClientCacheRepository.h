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
#ifndef ISTATISTICCLIENTCACHEREPOSITORY_H
#define ISTATISTICCLIENTCACHEREPOSITORY_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>
#include <QQueue>
#include <QPair>

// Project includes
#include "IStatisticEntry.h"
#include "../../Container/IError.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            /**
              * Handles Persistence of News and NewsImages
              */

            class IStatisticClientCacheRepository  {
                public:
                    virtual ~IStatisticClientCacheRepository() {}


                    virtual IStatisticEntry* createInMemoryEntry( QString entryType, QString data ) = 0;

                    /**
                      * Create new IStatisticClientCache object
                      *
                      * \param entryType type of the entry e.g. Player start or Podcaast start etc.
                      * \param data the additional data that is sent to the server.
                      * \param date typoe of the entry e.g. Player start or Podcaast start etc.
                      * \return IStatisticEntry* Pointer to IStatisticEntry object
                      */
                    virtual IStatisticEntry* createEntry( QString entryType, QString data ) = 0;

                    /**
                      * Try to delete the IStatisticClientCache object from the db
                      *
                      * \param guid The id of the entry
                      * \param error error container
                      * \return bool true/false
                      */
                    virtual bool deleteEntry( QUuid guid, RedBullPlayer::IError* error ) = 0;


                    /**
                      * Fetch all IStatisticEntry objects from db
                      *
                      * \return QList<IStatisticEntry*>* List of all IStatisticEntry objects
                      */
                    virtual QList<IStatisticEntry*>* getAllEntries() = 0;
            };
        }
    }
}
#endif // ISTATISTICCLIENTCACHEREPOSITORY_H
