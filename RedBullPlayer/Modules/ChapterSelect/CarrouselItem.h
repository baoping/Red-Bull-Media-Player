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
#ifndef CARROUSELITEM_H
#define CARROUSELITEM_H

#include "../../Widgets/ClickableLabel.h"

class CarrouselItem : public RedBullPlayer::Widgets::ClickableLabel {
    public:
        CarrouselItem( QImage img, int width, int height, QWidget *parent, qint64 chapterStartDurationSeconds, int videoLengthSeconds, int id = -1 );
        virtual ~CarrouselItem();
        void setFrameWidth( int width );
        void setHighlighted( bool highlighted );
        bool highLighted();
        void changeChapterImage( QImage img, bool immediateUpdate );

    protected:
        virtual void paintEvent( QPaintEvent *event );
        void setChapterStartToolTip( qint64 chapterStartDurationSeconds, int videoDurationSeconds );

    private:
        int _frameWidth;
        int _width;
        int _height;
        bool _highlighted;
        QRect *_imgGeometry;
        QPixmap _img;
};

#endif // CARROUSELITEM_H
