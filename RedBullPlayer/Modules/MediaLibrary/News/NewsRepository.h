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
#ifndef NEWSREPOSITORY_H
#define NEWSREPOSITORY_H

// Qt includes
#include <QObject>
#include <QUuid>
#include <QString>
#include <QMutex>
#include <QtSql>
#include <QSqlDatabase>

// Project includes
#include "../MediaDB.h"
#include "../../../Interfaces/MediaLibrary/INews.h"
#include "../../../Interfaces/MediaLibrary/INewsImage.h"
#include "../../../Interfaces/MediaLibrary/INewsRepository.h"
#include "../../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class NewsRepository : public QObject, public INewsRepository {
                    Q_OBJECT

                public:
                    NewsRepository( MediaDB* mediaDb, QObject* parent );
                    virtual ~NewsRepository();

                    virtual INews* createNews( const QUuid& guid, const QString& title );
                    virtual INewsImage* createNewsImage( const QUuid& newsGuid, const QUuid& guid, const QString& fileName );

                    virtual bool saveNews( const INews& news, RedBullPlayer::Container::Error* error );
                    virtual bool saveNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error );

                    virtual QList<INews*>* allNews( bool orderByPubDate = true );
                    virtual QList<INewsImage*>* allNewsImages( QUuid newsGuid );

                    virtual bool anyNewsImageExists( QUuid newsGuid );

                    virtual INews* getNewsByGuid( QUuid guid );
                    virtual INews* getNewsByExternalGuid( QString externalGuid );
                    virtual INewsImage* getNewsImageByFileName( QUuid newsGuid, QString fileName );

                    virtual bool deleteNews( const INews& news, RedBullPlayer::Container::Error* error );
                    virtual bool deleteNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error );
                    virtual void clearCache( RedBullPlayer::Container::Error *error );

                private: // Functions
                    INews* createNewsFromSQLResult( QSqlRecord record );
                    INewsImage* createNewsImageFromSQLResult( QSqlRecord record );

                    bool newsExists( const QUuid& guid, RedBullPlayer::Container::Error* error );
                    bool newsImageExists( const QUuid& guid, RedBullPlayer::Container::Error* error );

                    bool updateNews( const INews& news, RedBullPlayer::Container::Error* error );
                    bool insertNews( const INews& news, RedBullPlayer::Container::Error* error );

                    bool updateNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error );
                    bool insertNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error );

                    QSqlQuery mapNewsQuery( const INews& news, QString sql );
                    QSqlQuery mapNewsImageQuery( const INewsImage& newsImage, QString sql );

                private: // Data
                    MediaDB* _mediaDb;

                    mutable QMutex _createNewsMutex;
                    mutable QMutex _createNewsImageMutex;
                    mutable QMutex _saveNewsMutex;
                    mutable QMutex _saveNewsImageMutex;
                    mutable QMutex _allNewsMutex;
                    mutable QMutex _allNewsImagesMutex;
                    mutable QMutex _getNewsByGuidMutex;
                    mutable QMutex _getNewsByExternalGuidMutex;
                    mutable QMutex _getNewsImageByFileNameMutex;
                    mutable QMutex _deleteNewsMutex;
                    mutable QMutex _deleteNewsImageMutex;
                    mutable QMutex _createNewsFromSQLResultMutex;
                    mutable QMutex _createNewsImageFromSQLResultMutex;
                    mutable QMutex _newsExistsMutex;
                    mutable QMutex _newsImageExistsMutex;
                    mutable QMutex _clearCacheMutex;

            };
        }
    }
}

#endif // NEWSREPOSITORY_H
