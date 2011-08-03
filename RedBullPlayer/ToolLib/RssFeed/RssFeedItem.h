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
#ifndef RSSFEEDITEM_H
#define RSSFEEDITEM_H

#include <QString>
#include <QDateTime>

#include "../../Interfaces/RssFeed/IRssFeedItem.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace RSS {
            class RssFeedItem : public IRssFeedItem {
                public:
                    RssFeedItem();

                    //required elements
                    virtual QString title() const {
                        return _title;
                    }

                    virtual QString link() const {
                        return _link;
                    }

                    virtual QString description() const {
                        return _description;
                    }

                    virtual QString contentEncoded() const {
                        return _contentEncoded;
                    }

                    //optional elements (selection)
                    virtual QString author() const {
                        return _author;
                    }

                    virtual QString category() const {
                        return _category;
                    }

                    virtual QString comments() const {
                        return _comments;
                    }

                    virtual QString enclosure() const {
                        return _enclosure;
                    }

                    virtual QString enclosureContentType() const {
                        return _contentType;
                    }

                    virtual void setEnclosureContentType( QString contentType ) {
                        _contentType = contentType;
                    }

                    virtual QString enclosureContentLength() const {
                        return _contentLength;
                    }

                    virtual void setEnclosureContentLength( QString contentLength ) {
                        _contentLength = contentLength;
                    }

                    virtual QString guid();

                    virtual QDateTime pubDate() const {
                        return _pubDate;
                    }

                    virtual QString pubDateString() const {
                        return _pubDateString;
                    }

                    virtual QString source() const {
                        return _source;
                    }

                    //RedBull-Tags
                    virtual QString redBullImageUrl() const {
                        return _redBullImageUrl;
                    }

                    virtual QImage redBullImage() const {
                        return _redBullImage;
                    }

                    virtual void setRedBullImage( QString fileName ) {
                        _redBullImage = QImage( fileName );
                    }

                    virtual void setRedBullImage( QImage image ) {
                        _redBullImage = QImage( image );
                    }

                    /***** Setter *****/
                    void setTitle( QString title ) {
                        _title = title;
                    }

                    void setLink( QString link ) {
                        _link = link;
                    }

                    void setDescription( QString description ) {
                        _description = description;
                    }

                    void setContentEncoded( QString contentEncoded ) {
                        _contentEncoded = contentEncoded;
                    }

                    void setAuthor( QString author ) {
                        _author = author;
                    }

                    void setCategory( QString category ) {
                        _category = category;
                    }

                    void setComments( QString comments ) {
                        _comments = comments;
                    }

                    void setEnclosure( QString enclosure ) {
                        _enclosure = enclosure;
                    }

                    void setGuid( QString guid ) {
                        _guid = guid;
                    }

                    void setPubDate( QDateTime pubDate ) {
                        _pubDate = pubDate;
                    }

                    void setPubDateString( QString pubDateString ) {
                        _pubDateString = pubDateString;
                    }

                    void setSource( QString source ) {
                        _source = source;
                    }

                    void setRedBullImageUrl( QString url ) {
                        _redBullImageUrl = url;
                    }

                    //local filename
                    virtual QString localFileName() const {
                        return _localFileName;
                    }

                    virtual void setLocalFileName( QString fileName ) {
                        _localFileName = fileName;
                    }

                private:
                    QString _title;
                    QString _link;
                    QString _description;
                    QString _contentEncoded;
                    QString _author;
                    QString _category;
                    QString _comments;
                    QString _enclosure;
                    QString _guid;
                    QDateTime _pubDate;
                    QString _pubDateString;
                    QString _source;
                    QString _redBullImageUrl;
                    QString _contentType;
                    QString _contentLength;
                    QString _localFileName;
                    QImage _redBullImage;
            };
        }
    }
}


#endif // RSSFEEDITEM_H
