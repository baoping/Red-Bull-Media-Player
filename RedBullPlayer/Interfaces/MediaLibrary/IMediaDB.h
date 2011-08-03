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
#ifndef IMEDIADB_H
#define IMEDIADB_H

#include "IMediaType.h"
#include "ITagRepository.h"
#include "IMediaRepository.h"
#include "ISnapshotRepository.h"
#include "INewsRepository.h"
#include "ISystemMetaDataRepository.h"
#include "IStatisticClientCacheRepository.h"
#include "IContentRepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IMediaDB {
                public:
                    virtual ~IMediaDB() {}
                    virtual QString databaseName() = 0;
                    virtual const QList<IMediaType*>& mediaTypes() = 0;
                    virtual ITagRepository* tagRepository() const = 0;
                    virtual IMediaRepository* mediaRepository() const = 0;
                    virtual ISnapshotRepository* snapshotRepository() const = 0;
                    virtual INewsRepository* newsRepository() const = 0;
                    virtual ISystemMetaDataRepository* systemMetaDataRepository() const = 0;
                    virtual IStatisticClientCacheRepository* statisticClientCacheRepository() const = 0;
                    virtual IContentRepository* contentRepository() const = 0;

                    virtual IMediaType* videoType() const = 0;
                    virtual IMediaType* audioType() const = 0;
                    virtual IMediaType* imageType() const = 0;
                    virtual IMediaType* tvChannelType() const = 0;
                    virtual IMediaType* documentType() const = 0;

                    virtual void exportDataToXml( QString fileName ) = 0;
            };
        }
    }
}
#endif // IMEDIADB_H
