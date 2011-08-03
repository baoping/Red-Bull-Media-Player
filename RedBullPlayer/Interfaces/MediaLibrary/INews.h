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
#ifndef INEWS_H
#define INEWS_H

#include <QUuid>
#include <QString>
#include <QDateTime>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class INews {
                public:
                    virtual ~INews() {}

                    virtual const QUuid& guid() const = 0;

                    virtual QString title() const = 0;
                    virtual void setTitle( QString title ) = 0;

                    virtual QString link() const = 0;
                    virtual void setLink( QString link ) = 0;

                    virtual QString description() const = 0;
                    virtual void setDescription( QString description ) = 0;

                    virtual QString content() const = 0;
                    virtual void setContent( QString content ) = 0;

                    virtual QString author() const = 0;
                    virtual void setAuthor( QString author ) = 0;

                    virtual QString category() const = 0;
                    virtual void setCategory( QString category ) = 0;

                    virtual QString externalGuid() const = 0;
                    virtual void setExternalGuid( QString guid ) = 0;

                    virtual QDateTime pubDate() const = 0;
                    virtual void setPubDate( QDateTime pubDate ) = 0;

                    virtual bool isValid() const = 0;
            };
        }
    }
}

#endif // INEWS_H
