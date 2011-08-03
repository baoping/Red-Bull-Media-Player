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
#include "TransparentImage.h"

// QT includes
#include <QPainter>
#include <QDebug>

using namespace RedBullPlayer::Widgets;

TransparentImage::TransparentImage( QString imgUrl, int alpha, QWidget *parent ) :
    ClickableLabel( parent ) {
    setCursor( Qt::PointingHandCursor );
    _img = QImage( imgUrl );
    _alpha = alpha;
}

void TransparentImage::setAlpha( int newValue ) {
    _alpha = newValue;
    update();
}

void TransparentImage::paintEvent( QPaintEvent * /*event*/ ) {
    QPainter painter( this );
    QImage img = _img.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    /*QPoint p1, p2;
    p2.setY(img.height());
    QLinearGradient gradient(p1, p2);
    gradient.setColorAt(0, QColor(0, 0, 0, _alpha));
    gradient.setColorAt(1, QColor(0, 0, 0, _alpha));
    QPainter imgPainter(&img);
    imgPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    imgPainter.fillRect(0, 0, img.width(), img.height(), gradient);
    imgPainter.end();*/
    painter.drawImage( 0, 0, img );
}
