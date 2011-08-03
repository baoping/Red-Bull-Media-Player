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
#ifndef CONTENT_H
#define CONTENT_H

// Qt includes
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QUuid>

// Project includes
#include "../../../Interfaces/MediaLibrary/IContent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class Content : public IContent {
                public:
                    Content( const QString& guid,
                             const QString& name,
                             const QString& fileName,
                             const QString& downloadUrl );

                    virtual ~Content() {}
                    virtual const QString& guid() const {
                        return _guid;
                    }

                    virtual QUuid mediaGuid() const {
                        return _mediaGuid;
                    }
                    virtual void setMediaGuid( QUuid mediaGuid ) {
                        _mediaGuid = mediaGuid;
                    }

                    virtual QString name() const {
                        return _name;
                    }
                    virtual void setName( QString name ) {
                        _name = name;
                    }

                    virtual QString fileName() const {
                        return _fileName;
                    }
                    virtual void setFileName( QString fileName ) {
                        _fileName = fileName;
                    }

                    virtual QString downloadUrl() const {
                        return _downloadUrl;
                    }
                    virtual void setDownloadUrl( QString downloadUrl ) {
                        _downloadUrl = downloadUrl;
                    }

                    virtual uint fileSize() const {
                        return _fileSize;
                    }
                    virtual void setFileSize( uint fileSize ) {
                        _fileSize = fileSize;
                    }

                    virtual QDateTime date() const {
                        return _date;
                    }
                    virtual void setDate( QDateTime date ) {
                        _date = date;
                    }

                    virtual QString state() const {
                        return _state;
                    }
                    virtual void setState( QString state )  {
                        _state = state;
                    }

                private:
                    Content();

                    QUuid _mediaGuid;

                    QString _guid;
                    QString _fileName;
                    QString _name;
                    QString _downloadUrl;
                    uint _fileSize;
                    QDateTime _date;
                    QString _state;
            };
        }
    }
}

#endif // CONTENT_H
