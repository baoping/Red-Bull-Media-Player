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
#ifndef ICONTENTREPOSITORY_H
#define ICONTENTREPOSITORY_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>

// Project includes
#include "IContent.h"
#include "IMedia.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IContentRepository  {
                public:
                    virtual ~IContentRepository() {}
                    virtual IContent* createContent( const QString& guid,
                                                     const QString& name,
                                                     const QString& fileName,
                                                     const QString& downloadUrl ) = 0;
                    virtual bool saveContent( const IContent& content, RedBullPlayer::Container::Error* error ) = 0;
                    virtual IContent* getContentByGuid( QString guid ) = 0;
                    virtual QList<IContent*>* allContent() = 0;
                    virtual QList<IContent*>* allContentByState( QString state ) = 0;
                    virtual IContent* getContentByMediaGuid( QString mediaGuid ) = 0;
                    virtual bool deleteContent( const IContent& content, RedBullPlayer::Container::Error* error ) = 0;
            };
        }
    }
}

#endif // ICONTENTREPOSITORY_H
