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
#ifndef IRSSFEED_H
#define IRSSFEED_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QPixmap>

#include "IRssFeedImageDescriptor.h"
#include "IRssFeedItem.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace RSS {
            class IRssFeed  {
                public:
                    virtual ~IRssFeed() { }

                    //required channel elements
                    virtual QString title() const = 0;
                    virtual QString link() const = 0;
                    virtual QString description() const = 0;

                    //optional channel elements (selection)
                    virtual QString language() const = 0;
                    virtual QString copyright() const = 0;

                    virtual QString managingEditor() const = 0;
                    virtual QString webMaster() const = 0;
                    virtual QDateTime pubDate() const = 0;
                    virtual QString pubDateString() const = 0;
                    virtual QString category() const = 0;
                    virtual QString generator() const  = 0;
                    virtual IRssFeedImageDescriptor* imageDescriptor() const = 0;
                    virtual QImage image() const = 0;
                    virtual void setImage( QString fileName ) = 0;
                    virtual QString rating() const = 0;

                    virtual int itemCount() const = 0;
                    virtual IRssFeedItem* itemAt( int index ) = 0;
            };
        }
    }
}


#endif // IRSSFEED_H
