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
#include "Sprite.h"

// Qt Includes
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

using namespace RedBullPlayer::Widgets;

Sprite::Sprite( QWidget *parent, int id/* = -1*/ ) : ClickableLabel( parent, id ) {
    setAttribute( Qt::WA_Hover );
    _color = QColor( 255, 255, 255 );
}

void Sprite::paintEvent( QPaintEvent */*event*/ ) {
    QPainter painter( this );
    QBrush brush( _color );
    painter.setBrush( brush );
    painter.drawRect( 0, 0, width(), height() );
}

void Sprite::enterEvent( QEvent */*event*/ ) {
    if( isEnabled() ) {
        if( _id >= 0 ) {
            emit mouseOver( _id );
        } else {
            emit mouseOver();
        }
    }
}

void Sprite::leaveEvent( QEvent */*event*/ ) {
    if( isEnabled() ) {
        if( _id >= 0 ) {
            emit mouseOut( _id );
        } else {
            emit mouseOut();
        }
    }
}

void Sprite::setBgColor( QColor newColor ) {
    _color = newColor;
    update();
}
