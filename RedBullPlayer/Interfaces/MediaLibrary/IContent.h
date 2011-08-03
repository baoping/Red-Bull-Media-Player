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
#ifndef ICONTENT_H
#define ICONTENT_H

// Qt includes
#include <QUuid>
#include <QDateTime>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IContent {

                public:
                    virtual ~IContent() {}
                    virtual const QString& guid() const = 0;

                    virtual QUuid mediaGuid() const = 0;
                    virtual void setMediaGuid( QUuid mediaGuid ) = 0;

                    virtual QString name() const = 0;
                    virtual void setName( QString name ) = 0;

                    virtual QString fileName() const = 0;
                    virtual void setFileName( QString fileName ) = 0;

                    virtual QString downloadUrl() const = 0;
                    virtual void setDownloadUrl( QString downloadUrl ) = 0;

                    virtual uint fileSize() const = 0;
                    virtual void setFileSize( uint fileSize ) = 0;

                    virtual QDateTime date() const = 0;
                    virtual void setDate( QDateTime date ) = 0;

                    virtual QString state() const = 0;
                    virtual void setState( QString state ) = 0;
            };
        }
    }
}

#endif // ICONTENT_H
