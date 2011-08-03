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
#ifndef MEDIADB_H
#define MEDIADB_H

#include <QObject>
#include <QString>
#include <QtSql>
#include <QSqlDatabase>

#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/IMediaType.h"
#include "../../Interfaces/MediaLibrary/ITagRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/ISnapshotRepository.h"
#include "../../Interfaces/MediaLibrary/INewsRepository.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"
#include "../../Interfaces/MediaLibrary/IStatisticClientCacheRepository.h"
#include "../../Interfaces/MediaLibrary/IContentRepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class MediaDB : public QObject, public IMediaDB {
                public:
                    MediaDB( QObject* parent, const QString& databaseName = QString::null );

                    ~MediaDB();
                    // IMediaDB

                    virtual QString databaseName();

                    virtual const QList<IMediaType*>& mediaTypes();

                    virtual ITagRepository* tagRepository() const {
                        return _tagRepository;
                    }

                    virtual IMediaRepository* mediaRepository() const {
                        return _mediaRepository;
                    }

                    virtual ISnapshotRepository* snapshotRepository() const {
                        return _snapshotRepository;
                    }

                    virtual INewsRepository* newsRepository() const {
                        return _newsRepository;
                    }

                    virtual ISystemMetaDataRepository* systemMetaDataRepository() const {
                        return _systemMetaDataRepository;
                    }

                    virtual IStatisticClientCacheRepository* statisticClientCacheRepository() const {
                        return _statisticClientCacheRepository;
                    }

                    virtual IContentRepository* contentRepository() const {
                        return _contentRepository;
                    }

                    virtual IMediaType* videoType() const {
                        return _videoType;
                    }

                    virtual IMediaType* audioType()const {
                        return _audioType;
                    }

                    virtual IMediaType* imageType()const {
                        return _imageType;
                    }

                    virtual IMediaType* tvChannelType()const {
                        return _tvType;
                    }

                    virtual IMediaType* documentType()const {
                        return _documentType;
                    }

                    virtual void exportDataToXml( QString fileName );

                    // internals
                    QSqlDatabase db();
                    void ensureSchema();
                    void ensureDefaultData();

                private:
                    void ensureMediaType( QString id, QString name );
                    void ensureSystemMetaData( QString key, QString value );

                    QString _dbName;
                    QList<IMediaType*>* _mediaTypes;

                    IMediaType* _videoType;
                    IMediaType* _audioType;
                    IMediaType* _imageType;
                    IMediaType* _tvType;
                    IMediaType* _documentType;

                    ITagRepository* _tagRepository;
                    IMediaRepository* _mediaRepository;
                    ISnapshotRepository* _snapshotRepository;

                    INewsRepository* _newsRepository;
                    ISystemMetaDataRepository* _systemMetaDataRepository;
                    IStatisticClientCacheRepository* _statisticClientCacheRepository;
                    IContentRepository* _contentRepository;
            };
        }
    }
}
#endif // MEDIADB_H
