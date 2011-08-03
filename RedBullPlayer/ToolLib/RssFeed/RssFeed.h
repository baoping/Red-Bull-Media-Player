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
#ifndef RSSFEED_H
#define RSSFEED_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QtXml/QXmlStreamReader>

#include "../../Interfaces/RssFeed/IRssFeedImageDescriptor.h"
#include "../../Interfaces/RssFeed/IRssFeedItem.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace RSS {
            class RssFeed : public IRssFeed  {
                public:
                    RssFeed( QByteArray rawFeedData );
                    virtual ~RssFeed();

                    //required channel elements
                    virtual QString title() const {
                        return _title;
                    }

                    virtual QString link() const {
                        return _link;
                    }

                    virtual QString description() const {
                        return _description;
                    }

                    //optional channel elements (selection)
                    virtual QString language() const {
                        return _language;
                    }

                    virtual QString copyright() const {
                        return _copyright;
                    }

                    virtual QString managingEditor() const {
                        return _managingEditor;
                    }

                    virtual QString webMaster() const {
                        return _webMaster;
                    }

                    virtual QDateTime pubDate() const {
                        return _pubDate;
                    }

                    virtual QString pubDateString() const {
                        return _pubDateString;
                    }

                    virtual QString category() const {
                        return _category;
                    }

                    virtual QString generator() const {
                        return _generator;
                    }

                    virtual IRssFeedImageDescriptor* imageDescriptor() const {
                        return _imageDescriptor;
                    }

                    virtual QImage image() const {
                        return _image;
                    }

                    virtual void setImage( QString fileName ) {
                        _image = QImage( fileName );
                    }

                    virtual QString rating() const {
                        return _rating;
                    }

                    virtual int itemCount() const {
                        return _items->count();
                    }

                    virtual IRssFeedItem* itemAt( int index ) {
                        return _items->at( index );
                    }

                    //static
                    static QDateTime dateTimeFromRFC822( QString dateTimeStr );
                    static IRssFeed* parseFeed( QByteArray rawFeedData );

                private: //functions
                    void parseRawData( QByteArray rawData );
                    void parseImageElement( QXmlStreamReader* xml );
                    void parseItemElement( QXmlStreamReader* xml );

                private: //fields
                    QString _title;
                    QString _link;
                    QString _description;
                    QString _language;
                    QString _copyright;
                    QString _managingEditor;
                    QString _webMaster;
                    QDateTime _pubDate;
                    QString _pubDateString;
                    QString _category;
                    QString _generator;
                    IRssFeedImageDescriptor* _imageDescriptor;
                    QString _rating;
                    QImage _image;

                    QList<IRssFeedItem*>* _items;
            };
        }
    }
}

#endif // RSSFEED_H
