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
#ifndef IMAGELISTHANDLER_H
#define IMAGELISTHANDLER_H

#include <QPixmap>
#include <QMap>
#include <QSize>

#include "Enumerations.h"

namespace RedBullPlayer {
    namespace Widgets {
        class ImageListHandler {
            public:
                ImageListHandler();
                virtual ~ImageListHandler();

            private:
                QMap<ButtonImageType, QPixmap*>* _images;

            protected:
                QSize _imageSize;
                QPixmap getImage( ButtonImageType imageType );
                void addImage( ButtonImageType imageType, QString imageUrl );
        };
    }
}

#endif // IMAGELISTHANDLER_H
