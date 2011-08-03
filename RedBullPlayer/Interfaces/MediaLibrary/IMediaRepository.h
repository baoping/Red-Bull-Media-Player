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
#ifndef IMEDIAREPOSITORY_H
#define IMEDIAREPOSITORY_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>
#include <QQueue>
#include <QPair>

// Project includes
#include "ITag.h"
#include "IMedia.h"
#include "IMediaType.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IMediaRepository  {
                public:
                    virtual ~IMediaRepository() {}
                    virtual IMedia* createNewMedia( const QUuid& guid, const QString& fileName, const IMediaType& mediaType ) = 0;
                    virtual bool saveMedia( const IMedia& media, RedBullPlayer::Container::Error* error ) = 0;
                    virtual IMedia* getMediaByGuid( QUuid guid ) = 0;
                    virtual IMedia* getMediaByFileName( QString fileName ) = 0;
                    virtual QList<IMedia*>* allMedia() = 0;
                    virtual QList<QPair<QUuid, QString>*>* allMediaInUIOrder() = 0;
                    virtual QList<IMedia*>* getMediaByMediaType( const IMediaType& mediaType ) = 0;
                    virtual QList<IMedia*>* getMediaByTag( const ITag& tag ) = 0;
                    virtual bool addTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error ) = 0;
                    virtual bool removeTag( const IMedia& media, const ITag& tag, RedBullPlayer::Container::Error* error ) = 0;
                    virtual bool deleteMedia( const IMedia& media, RedBullPlayer::Container::Error* error ) = 0;
                    virtual bool isOnlineContent( QUuid guid ) = 0;
                    virtual QList<QUuid>* getMediaUsingDBSearch( QString query ) = 0;
            };
        }
    }
}


#endif // IMEDIAREPOSITORY_H
