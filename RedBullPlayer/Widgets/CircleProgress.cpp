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
#include "CircleProgress.h"

// Qt Includes
#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QFont>
#include <math.h>


using namespace RedBullPlayer::Widgets;

CircleProgress::CircleProgress( QWidget *parent,
                                QPixmap* pointImage ) : QLabel( parent ) {
    _pointImage = new QPixmap( pointImage->scaled( 20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation ) );
    _min = 0;
    _max = 100;
    _pos = 0;
    _percent = 0;
    _step = 10;
    _circleCount = 0;
}

CircleProgress::~CircleProgress() {
}

QSize CircleProgress::sizeHint() const {
    return QSize( 354, 200 );
}

bool CircleProgress::setPosition( double position ) {
    if ( ( position >= _min ) && ( position <= _max ) ) {
        _pos = position;
        double p = ( ( _pos - _min ) ) * 100 / ( _max - _min );
        setPercent( ( int ) p );
        return true;
    }

    return false;
}

bool CircleProgress::setPercent( int percent ) {
    if ( ( percent >= 0 ) && ( percent <= 100 ) ) {
        _percent = percent;
        updateCircleCount();
        return true;
    }

    return false;
}

void CircleProgress::updateCircleCount() {
    double count = ( 12 * ( double )_percent ) / 100;
    _circleCount = ( int ) round( count );
    update();
}

bool CircleProgress::doStep() {
    return setPosition( _pos + _step );
}

void CircleProgress::setText( QString text ) {
    if ( _text.compare( text ) != 0 ) {
        _text = text;
        update();
    }
}

void CircleProgress::doProgress( double position, QString text ) {
    _text = text;
    setPosition( position );
}

void CircleProgress::paintEvent( QPaintEvent */*event*/ ) {
    QPainter painter( this );
    //    int cx = 177;
    //    int cy = 80;
    int cx = ( int ) width() / 2;
    int cy = ( ( int ) height() / 2 ) - 25;
    int r = 60;
    int modx = ( int ) _pointImage->width() / 2;
    int mody = ( int ) _pointImage->height() / 2;
    int i;
    int angle = 90;
    cx = cx - modx;
    cy = cy - mody;

    for( i = 0; i < _circleCount; i++ ) {
        double x = cx + ( r * cos( angle * M_PI / 180 ) );
        double y = cy + ( r * sin( angle * M_PI / 180 ) );
        painter.drawPixmap(
            ( int )x,
            ( int )y,
            *_pointImage );

        if ( angle == 330 ) {
            angle = 0;
        } else {
            angle = angle + 30;
        }
    }

    if ( _percent > 0 ) {
        QPen pen( QColor( 255, 255, 255 ) );
        painter.setPen( pen );
        painter.setFont( QFont( "Arial", 10, 0, 0 ) );
        QRect pTextRect( 0, cy , width(), _pointImage->height() );
        painter.drawText( pTextRect,
                          Qt::AlignCenter,
                          QString( "%1\%" ).arg( _percent ) );
        painter.drawText(
            ( ( int ) width() / 2 ) - r - ( ( int ) _pointImage->width() / 2 ),
            cy + r + 40,
            260,
            25, ( Qt::AlignLeft | Qt::AlignVCenter ), _text );
    }
}
