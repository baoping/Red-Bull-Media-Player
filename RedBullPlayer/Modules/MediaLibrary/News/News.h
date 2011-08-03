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
#ifndef NEWS_H
#define NEWS_H

// Qt includes
#include <QString>
#include <QUuid>
#include <QDateTime>

// Project includes
#include "../../../Interfaces/MediaLibrary/INews.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class News : public INews {
                public:
                    News( const QUuid& guid, const QString& title );
                    News( const INews& news );
                    virtual ~News();

                    virtual const QUuid& guid() const {
                        return _guid;
                    }

                    virtual QString title() const {
                        return _title;
                    }
                    virtual void setTitle( QString title ) {
                        _title = title;
                    }
                    virtual QString link() const {
                        return _link;
                    }
                    virtual void setLink( QString link ) {
                        _link = link;
                    }
                    virtual QString description() const {
                        return _description;
                    }
                    virtual void setDescription( QString description ) {
                        _description = description;
                    }
                    virtual QString content() const {
                        return _content;
                    }
                    virtual void setContent( QString content ) {
                        _content = content;
                    }
                    virtual QString author() const {
                        return _author;
                    }
                    virtual void setAuthor( QString author ) {
                        _author = author;
                    }
                    virtual QString category() const {
                        return _category;
                    }
                    virtual void setCategory( QString category ) {
                        _category = category;
                    }
                    virtual QString externalGuid() const {
                        return _externalGuid;
                    }
                    virtual void setExternalGuid( QString guid ) {
                        _externalGuid = guid;
                    }
                    virtual QDateTime pubDate() const {
                        return _pubDate;
                    }
                    virtual void setPubDate( QDateTime pubDate ) {
                        _pubDate = pubDate;
                    }

                    virtual bool isValid() const;

                private: //functions
                    News();
                private: //data
                    QUuid _guid;
                    QString _title;
                    QString _link;
                    QString _description;
                    QString _content;
                    QString _author;
                    QString _category;
                    QString _externalGuid;
                    QDateTime _pubDate;
            };
        }
    }
}

#endif // NEWS_H
