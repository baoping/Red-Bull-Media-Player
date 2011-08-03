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
#ifndef INEWSREPOSITORY_H
#define INEWSREPOSITORY_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>
#include <QQueue>
#include <QPair>

// Project includes
#include "INews.h"
#include "INewsImage.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            /**
              * Handles Persistence of News and NewsImages
              */
            class INewsRepository  {
                public:
                    virtual ~INewsRepository() {}
                    /**
                      * Create new INews object
                      *
                      * \param guid Identifier
                      * \param title Title
                      * \return INews* Pointer to INews object
                      */
                    virtual INews* createNews( const QUuid& guid, const QString& title ) = 0;
                    /**
                      * Create new INewsImage object
                      *
                      * \param newsGuid Identifier of News Object (for association)
                      * \param guid Identifier
                      * \param fileName FileName to Image
                      * \return INews* Pointer to INewsImage object
                      */
                    virtual INewsImage* createNewsImage( const QUuid& newsGuid, const QUuid& guid, const QString& fileName ) = 0;

                    /**
                      * Save a INews object to db
                      *
                      * \param news news The Object to store
                      * \param error Error container
                      * \return bool true/false
                      */
                    virtual bool saveNews( const INews& news, RedBullPlayer::Container::Error* error ) = 0;
                    /**
                      * Save a INewsImage object to db
                      *
                      * \param INewsImage& news The Object to store
                      * \param Error* Error container
                      * \return bool true/false
                      */
                    virtual bool saveNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error ) = 0;

                    /**
                      * Fetch all INews objects from db
                      *
                      * \param orderByPubDate Sort result by PubDate
                      * \return QList<INews*>* List of all INews objects
                      */
                    virtual QList<INews*>* allNews( bool orderByPubDate = true ) = 0;
                    /**
                      * Fetch all INewsImage objects from db
                      *
                      * \param newsGuid Identifier of associated News object
                      * \return QList<INewsImage*>* List of all INewsImage associated to News object
                      */
                    virtual QList<INewsImage*>* allNewsImages( QUuid newsGuid ) = 0;
                    /**
                      * Check if any NewsImage for News exists
                      *
                      * \param newsGuid Identifier of news
                      * \return true/false
                      */
                    virtual bool anyNewsImageExists( QUuid newsGuid ) = 0;
                    /**
                      * Get a single INews object by its identifier
                      *
                      * \param guid The identifier
                      * \return INews* The Object or 0
                      */
                    virtual INews* getNewsByGuid( QUuid guid ) = 0;
                    /**
                      * Get a single INews object by its external identifier
                      *
                      * \param externalGuid The external identifier
                      * \return INews* The Object or 0
                      */
                    virtual INews* getNewsByExternalGuid( QString externalGuid ) = 0;
                    /**
                      * Get a single INewsImage object by its filename and associated News Identifier
                      *
                      * \param newsGuid The News identifer
                      * \param fileName The Filename
                      * \return INewsImage* The Object or 0
                      */
                    virtual INewsImage* getNewsImageByFileName( QUuid newsGuid, QString fileName ) = 0;

                    /**
                      * Try to delete the INews object from the db
                      *
                      * \param news The Object
                      * \param error error container
                      * \return bool true/false
                      */
                    virtual bool deleteNews( const INews& news, RedBullPlayer::Container::Error* error ) = 0;
                    /**
                      * Try to delete the INewsImage object from the db
                      *
                      * \param newsImage The Object
                      * \param error error container
                      * \return bool true/false
                      */
                    virtual bool deleteNewsImage( const INewsImage& newsImage, RedBullPlayer::Container::Error* error ) = 0;
                    /**
                      * Clears the news cache
                      *
                      * \param error error container
                      */
                    virtual void clearCache( RedBullPlayer::Container::Error* error ) = 0;
            };
        }
    }
}

#endif // INEWSREPOSITORY_H
