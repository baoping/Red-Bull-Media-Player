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
#include "ClickableLabel.h"

// Qt Includes
#include <QDebug>
#include <QMouseEvent>

using namespace RedBullPlayer::Widgets;

ClickableLabel::ClickableLabel( QWidget *parent, int id /*= -1*/ ) : QLabel( parent ) {
    setCursor( Qt::PointingHandCursor );
    _id = id;
}

void ClickableLabel::mouseReleaseEvent ( QMouseEvent * event ) {
    QPoint mousePos = event->pos();

    if( mousePos.x() > 0 && mousePos.x() < width() && mousePos.y() > 0 && mousePos.y() < height() ) {
        if( _id >= 0 ) {
            emit clicked( _id );
        } else {
            emit clicked();
        }
    }
}

void ClickableLabel::setEnabled( bool enabled ) {
    QWidget::setEnabled( enabled );

    if( enabled ) {
        setCursor( Qt::PointingHandCursor );
    } else {
        setCursor( Qt::ArrowCursor );
    }
}

bool ClickableLabel::isEnabled() {
    return QWidget::isEnabled();
}
