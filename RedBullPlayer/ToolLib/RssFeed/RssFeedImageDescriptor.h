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
#ifndef RSSFEEDIMAGEDESCRIPTOR_H
#define RSSFEEDIMAGEDESCRIPTOR_H

#include <QString>

#include "../../Interfaces/RssFeed/IRssFeedImageDescriptor.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace RSS {
            class RssFeedImageDescriptor : public IRssFeedImageDescriptor {
                public:
                    RssFeedImageDescriptor( QString url,
                                            QString title,
                                            QString link,
                                            int width = 0,
                                            int height = 0 );

                    virtual QString url() const {
                        return _url;
                    }

                    virtual QString title() const {
                        return _title;
                    }

                    virtual QString link() const {
                        return _link;
                    }

                    //optional elements
                    virtual int width() const {
                        return _width;
                    }

                    virtual int height() const {
                        return _height;
                    }

                private:
                    QString _url;
                    QString _title;
                    QString _link;
                    int _width;
                    int _height;
            };
        }
    }
}

#endif // RSSFEEDIMAGEDESCRIPTOR_H
