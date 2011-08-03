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
#include "PlayHead.h"

// Qt Includes
#include <QPainter>

// Project Includes
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Container;

PlayHead::PlayHead( QWidget *parent ) : QWidget( parent ),
    _img( Constants::SEEK_BAR_IMAGE_DIR + Constants::SEEK_BAR_PLAY_HEAD ) {
    _oldPoint = QPoint( -1, -1 );
    _draggingOn = false;
    setMouseTracking( true );
    this->resize( _img.size() );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

QSize PlayHead::sizeHint() const {
    return QSize( _img.width(), _img.height() );
}

void PlayHead::paintEvent( QPaintEvent * /*e*/ ) {
    QPainter painter( this );
    painter.drawImage( 0, 0, _img );
}

void PlayHead::mousePressEvent ( QMouseEvent * ev ) {
    _draggingOn = true;
    _oldPoint = ev->globalPos();
    emit draggingStart();
}

void PlayHead::mouseMoveEvent ( QMouseEvent * ev ) {
    if( _draggingOn && _oldPoint.x() != -1 && _oldPoint.y() != -1 ) {
        QPoint distance = QPoint( ev->globalX() - _oldPoint.x(), ev->globalY() - _oldPoint.y() );
        emit dragging( distance );
    }

    _oldPoint.setX( ev->globalX() );
    _oldPoint.setY( ev->globalY() );
}

void PlayHead::mouseReleaseEvent ( QMouseEvent *  ) {
    _draggingOn = false;
    emit draggingEnd();
}
