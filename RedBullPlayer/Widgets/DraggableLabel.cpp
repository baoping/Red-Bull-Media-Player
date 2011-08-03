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
#include "DraggableLabel.h"

using namespace RedBullPlayer::Widgets;

// QT includes
#include <QDebug>
#include <QMouseEvent>

DraggableLabel::DraggableLabel( QWidget *parent ) : QLabel( parent ) {
    _oldPoint = QPoint( -1, -1 );
    _draggingOn = false;
    this->setMouseTracking( true );
}

void DraggableLabel::mousePressEvent ( QMouseEvent * ev ) {
    if( ev->button() == Qt::LeftButton ) {
        _draggingOn = true;
        _oldPoint = ev->globalPos();
        emit clicked();
        setCursor( QCursor( Qt::SizeAllCursor ) );
    }
}

void DraggableLabel::mouseMoveEvent ( QMouseEvent * ev ) {
    if( ev->buttons() == Qt::LeftButton ) {
        if( _draggingOn && _oldPoint.x() != -1 && _oldPoint.y() != -1 ) {
            QPoint distance = QPoint( ev->globalX() - _oldPoint.x(), ev->globalY() - _oldPoint.y() );
            emit dragging( distance );
        }

        _oldPoint.setX( ev->globalX() );
        _oldPoint.setY( ev->globalY() );
    } else {
        setCursor( QCursor( Qt::ArrowCursor ) );
    }
}

void DraggableLabel::mouseReleaseEvent ( QMouseEvent *  ) {
    _draggingOn = false;
    setCursor( QCursor( Qt::ArrowCursor ) );
}

void DraggableLabel::invokeMouseEvent( MouseEventType type, QMouseEvent *ev ) {
    switch( type ) {
        case DraggableLabel::PRESS:
            mousePressEvent( ev );
            break;

        case DraggableLabel::MOVE:
            mouseMoveEvent( ev );
            break;

        case DraggableLabel::RELEASE:
            mouseReleaseEvent( ev );
            break;
    }
}

void DraggableLabel::mouseDoubleClickEvent ( QMouseEvent * /*ev*/ ) {
    emit doubleClicked();
}

