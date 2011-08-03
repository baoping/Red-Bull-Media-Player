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
#ifndef SNAPSHOTREPOSITORY_H
#define SNAPSHOTREPOSITORY_H

#include <QObject>
#include <QUuid>
#include <QImage>
#include <QMutex>

// Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Interfaces/MediaLibrary/ITagRepository.h"
#include "../../Container/Error.h"
#include "MediaDB.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class SnapshotRepository : public QObject, public ISnapshotRepository {
                    Q_OBJECT
                public:
                    SnapshotRepository( MediaDB* mediaDb, QObject* parent );
                    virtual ISnapshot* createSnapshot( QUuid guid, QImage image, float position, IMedia* media );
                    virtual bool saveSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error );
                    virtual bool deleteSnapshot( const ISnapshot& snapshot, RedBullPlayer::Container::Error* error );
                    virtual ISnapshot* getSnapshotByGuid( QUuid guid, RedBullPlayer::Container::Error* error );
                    virtual ISnapshot* getSnapshotsByMediaAnPosition( IMedia* media, float position, RedBullPlayer::Container::Error* error );
                    virtual QList<ISnapshot*>* getSnapshotsByMedia( IMedia* media, RedBullPlayer::Container::Error* error );

                private: // Methods
                    ISnapshot* createSnapshotFromSQLresult( QSqlRecord record, IMedia* media, RedBullPlayer::Container::Error* error );
                private: // Data
                    MediaDB* _mediaDb;
                    mutable QMutex _createSnapshotMutex;
                    mutable QMutex _saveSnapshotMutex;
                    mutable QMutex _deleteSnapshotMutex;
                    mutable QMutex _getSnapshotByGuidMutex;
                    mutable QMutex _getSnapshotsByMediaAnPositionMutex;
                    mutable QMutex _getSnapshotsByMediaMutex;
                    mutable QMutex _createSnapshotFromSQLresultMutex;
            };
        }
    }
}

#endif // SNAPSHOTREPOSITORY_H
