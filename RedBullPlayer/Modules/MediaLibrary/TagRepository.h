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
#ifndef TAGREPOSITORY_H
#define TAGREPOSITORY_H

// Qt includes
#include <QObject>
#include <QString>
#include <QUuid>
#include <QList>
#include <QSqlRecord>
#include <QMutex>

// Project includes
#include "MediaDB.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Interfaces/MediaLibrary/ITagRepository.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class TagRepository : public QObject, public ITagRepository {
                    Q_OBJECT
                public:
                    TagRepository( MediaDB* mediaDb, QObject* parent );

                    // ITagRepository
                    virtual ITag* getTagByGuid( QUuid guid );
                    virtual ITag* getTagByName( QString name );
                    virtual bool saveTag( const ITag& tag, RedBullPlayer::Container::Error* error );
                    virtual bool deleteTag( const ITag& tag, RedBullPlayer::Container::Error* error );
                    virtual QList<ITag*>* getTagsByNameQuery( QString nameQuery );
                    virtual QList<ITag*>* tags();
                    virtual QList<ITag*>* getTagsByMedia( const IMedia& media );

                private: // Methods
                    ITag* createTagFromSQLresult( QSqlRecord record );
                private: // Data
                    MediaDB* _mediaDb;
                    mutable QMutex _getTagByGuidMutex;
                    mutable QMutex _getTagByNameMutex;
                    mutable QMutex _saveTagMutex;
                    mutable QMutex _deleteTagMutex;
                    mutable QMutex _getTagsByNameQueryMutex;
                    mutable QMutex _tagsMutex;
                    mutable QMutex _getTagsByMediaMutex;
                    mutable QMutex _createTagFromSQLresultMutex;

            };
        }
    }
}
#endif // TAGREPOSITORY_H
