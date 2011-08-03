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
#ifndef INEWSIMAGE_H
#define INEWSIMAGE_H

#include <QUuid>
#include <QString>
#include <QDateTime>
#include <QImage>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class INewsImage {
                public:
                    virtual ~INewsImage() {}

                    virtual const QUuid& guid() const = 0;
                    virtual const QUuid& newsGuid() const = 0;

                    virtual QString fileName() const = 0;
                    virtual void setFileName( QString fileName ) = 0;

                    virtual int width() const = 0;
                    virtual void setWidth( int width ) = 0;

                    virtual int height() const = 0;
                    virtual void setHeight( int height ) = 0;

                    virtual int size() const = 0;
                    virtual void setSize( int size ) = 0;

                    virtual QImage image() const = 0;
                    virtual void setImage( QImage image ) = 0;
            };
        }
    }
}

#endif // INEWSIMAGE_H
