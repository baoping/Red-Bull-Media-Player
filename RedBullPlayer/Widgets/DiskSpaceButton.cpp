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
#include "DiskSpaceButton.h"

#include <QPainter>

using namespace RedBullPlayer::Widgets;

DiskSpaceButton::DiskSpaceButton( QString normalImageUrl,
                                  QString progressImageUrl,
                                  QWidget *parent,
                                  int borderLeft,
                                  int borderRight,
                                  int borderTop,
                                  int borderBottom )
    : StatusDisplayButton( normalImageUrl, normalImageUrl, parent ) {
    _progressImage = new QPixmap( progressImageUrl );
    _percent = 0;
    _borderLeft = borderLeft;
    _borderRight = borderRight;
    _borderTop = borderTop;
    _borderBottom = borderBottom;
}

void DiskSpaceButton::paintEvent( QPaintEvent* event ) {
    StatusDisplayButton::paintEvent( event );
    QPainter painter( this );
    int x = 0 + _borderLeft;
    int y = 0 + _borderTop;
    int w = width() - _borderRight - _borderLeft;
    int h = height() - _borderBottom - _borderTop;
    QRect pTextRect( 0 + _borderLeft,
                     y,
                     w,
                     h );
    double mc = pTextRect.width() / _progressImage->width();
    int maxCount = ( int )mc;
    double c = ( maxCount * _percent ) / 100;
    int count = ( int )c;

    for( int i = 0; i < count; i++ ) {
        painter.drawPixmap( x, y, _progressImage->width(), h, *_progressImage );
        x += _progressImage->width();
    }

    painter.setPen( QColor( Qt::white ) );
    painter.drawText( pTextRect,
                      Qt::AlignCenter,
                      QString( "%1\%" ).arg( _percent ) );
}
