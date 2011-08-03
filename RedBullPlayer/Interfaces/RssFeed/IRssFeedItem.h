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
#ifndef IRSSFEEDITEM_H
#define IRSSFEEDITEM_H

#include <QString>
#include <QDateTime>
#include <QPixmap>

namespace RedBullPlayer {
    namespace Tools {
        namespace RSS {
            class IRssFeedItem  {
                public:
                    virtual ~IRssFeedItem() { }

                    //required elements
                    virtual QString title() const = 0;
                    virtual QString link() const = 0;
                    virtual QString description() const = 0;
                    virtual QString contentEncoded() const = 0;

                    //optional elements (selection)
                    virtual QString author() const = 0;
                    virtual QString category() const = 0;
                    virtual QString comments() const = 0;
                    virtual QString enclosure() const = 0;
                    virtual QString enclosureContentType() const = 0;
                    virtual void setEnclosureContentType( QString contentType ) = 0;
                    virtual QString enclosureContentLength() const = 0;
                    virtual void setEnclosureContentLength( QString contentLength ) = 0;
                    virtual QString guid() = 0;
                    virtual QDateTime pubDate() const = 0;
                    virtual QString pubDateString() const = 0;
                    virtual QString source() const = 0;

                    //RedBull-Tags
                    virtual QString redBullImageUrl() const = 0;
                    virtual QImage redBullImage() const = 0;
                    virtual void setRedBullImage( QString fileName ) = 0;
                    virtual void setRedBullImage( QImage image ) = 0;

                    //local filename
                    virtual QString localFileName() const = 0;
                    virtual void setLocalFileName( QString fileName ) = 0;
            };
        }
    }
}

#endif // IRSSFEEDITEM_H
