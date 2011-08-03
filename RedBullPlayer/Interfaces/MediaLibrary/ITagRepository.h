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
#ifndef ITAGREPOSITORY_H
#define ITAGREPOSITORY_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>

// Project includes
#include "ITag.h"
#include "IMedia.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class ITagRepository  {
                public:
                    virtual ~ITagRepository() {}
                    virtual ITag* getTagByGuid( QUuid guid ) = 0;
                    virtual ITag* getTagByName( QString name ) = 0;
                    virtual bool saveTag( const ITag& tag, RedBullPlayer::Container::Error* error ) = 0;
                    virtual bool deleteTag( const ITag& tag, RedBullPlayer::Container::Error* error ) = 0;
                    virtual QList<ITag*>* getTagsByNameQuery( QString nameQuery ) = 0;
                    virtual QList<ITag*>* tags() = 0;
                    virtual QList<ITag*>* getTagsByMedia( const IMedia& media ) = 0;

            };
        }
    }
}
#endif // ITAGREPOSITORY_H
