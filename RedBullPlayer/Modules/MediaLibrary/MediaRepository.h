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
#ifndef MEDIAREPOSITORY_H
#define MEDIAREPOSITORY_H

// Qt includes
#include <QObject>
#include <QString>
#include <QUuid>
#include <QList>
#include <QMutex>
#include <QMap>
// Project includes
#include "MediaDB.h"

#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/MediaLibrary/IMediaType.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Interfaces/MediaLibrary/ITagRepository.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class MediaRepository : public QObject, public IMediaRepository {
                    Q_OBJECT

                public:
                    MediaRepository( MediaDB* mediaDb, QObject* parent );
                    virtual ~MediaRepository();
                    virtual bool saveMedia( const IMedia& media, RedBullPlayer::Container::Error* error );
                    virtual IMedia* getMediaByGuid( QUuid guid );
                    virtual IMedia* getMediaByFileName( QString fileName );
                    virtual QList<IMedia*>* allMedia();
                    virtual QList<QPair<QUuid, QString>*>* allMediaInUIOrder();
                    virtual QList<IMedia*>* getMediaByMediaType( const IMediaType& mediaType );
                    virtual QList<IMedia*>* getMediaByTag( const ITag& tag );
                    virtual bool addTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error );
                    virtual bool removeTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error );
                    virtual bool deleteMedia( const IMedia& media, RedBullPlayer::Container::Error* error );
                    virtual IMedia* createNewMedia( const QUuid& guid, const QString& fileName, const IMediaType& mediaType );
                    virtual bool isOnlineContent( QUuid guid );
                    virtual QList<QUuid>* getMediaUsingDBSearch( QString query );

                private: // Methods
                    IMedia* createMediaFromSQLresult( QSqlRecord record );

                private: // Data
                    MediaDB* _mediaDb;
                    mutable QMutex _saveMediaMutex;
                    mutable QMutex _getMediaByGuidMutex;
                    mutable QMutex _getMediaByFileNameMutex;
                    mutable QMutex _allMediaMutex;
                    mutable QMutex _getMediaByMediaTypeMutex;
                    mutable QMutex _getMediaByTagMutex;
                    mutable QMutex _addTagMutex;
                    mutable QMutex _removeTagMutex;
                    mutable QMutex _deleteMediaMutex;
                    mutable QMutex _createNewMediaMutex;
                    mutable QMutex _isOnlineContentMutex;
                    mutable QMutex _createMediaFromSQLresultMutex;
                    mutable QMutex _allMediaInUIOrderMutex;
                    mutable QMutex _getMediaUsingDBSearchMutex;
            };
        }
    }
}
#endif // MEDIAREPOSITORY_H
