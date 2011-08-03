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
#ifndef ISNAPSHOTREPOSITORY_H
#define ISNAPSHOTREPOSITORY_H


// Qt includes
#include <QUuid>
#include <QString>
#include <QList>

// Project includes
#include "ITag.h"
#include "IMedia.h"
#include "ISnapshot.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class ISnapshotRepository  {
                public:
                    virtual ~ISnapshotRepository() {}
                    virtual ISnapshot* createSnapshot( QUuid guid, QImage image, float position, IMedia* media ) = 0;
                    virtual bool saveSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error ) = 0;
                    virtual bool deleteSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error ) = 0;

                    virtual ISnapshot* getSnapshotByGuid( QUuid guid, RedBullPlayer::Container::Error* error ) = 0;
                    virtual QList<ISnapshot*>* getSnapshotsByMedia( IMedia* media, RedBullPlayer::Container::Error* error ) = 0;
                    virtual ISnapshot* getSnapshotsByMediaAnPosition( IMedia* media, float position, RedBullPlayer::Container::Error* error ) = 0;
            };
        }
    }
}


#endif // ISNAPSHOTREPOSITORY_H
