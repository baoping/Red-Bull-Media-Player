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
#ifndef NEWSIMAGE_H
#define NEWSIMAGE_H

// Qt includes
#include <QString>
#include <QUuid>
#include <QImage>

// Project includes
#include "../../../Interfaces/MediaLibrary/INewsImage.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            class NewsImage : public INewsImage {
                public:
                    NewsImage( const QUuid& newsGuid, const QUuid& guid, const QString& fileName );
                    NewsImage( const INewsImage& newsImage );
                    virtual ~NewsImage();

                    virtual const QUuid& guid() const {
                        return _guid;
                    }
                    virtual const QUuid& newsGuid() const {
                        return _newsGuid;
                    }

                    virtual QString fileName() const {
                        return _fileName;
                    }
                    virtual void setFileName( QString fileName ) {
                        _fileName = fileName;
                    }

                    virtual int width() const {
                        return _width;
                    }
                    virtual void setWidth( int width ) {
                        _width = width;
                    }
                    virtual int height() const {
                        return _height;
                    }
                    virtual void setHeight( int height ) {
                        _height = height;
                    }

                    virtual int size() const {
                        return _size;
                    }
                    virtual void setSize( int size ) {
                        _size = size;
                    }

                    virtual QImage image() const {
                        return _image;
                    }
                    virtual void setImage( QImage image ) {
                        _image = image;
                    }
                private: //data
                    QUuid _guid;
                    QUuid _newsGuid;
                    QString _fileName;
                    int _width;
                    int _height;
                    int _size;
                    QImage _image;
            };

        } // namespace MediaLibrary
    } // namespace Modules
} // namespace RedBullPlayer

#endif // NEWSIMAGE_H
