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
#include "StatusDisplayButton.h"

//Project includes
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


StatusDisplayButton::StatusDisplayButton( QString normalImageUrl,
        QString disabledImageUrl,
        QWidget *parent, int width, int height )
    : CommandButton( parent, normalImageUrl, width, height ) {
    CommandButton::setDisabledImage( disabledImageUrl );
    CommandButton::disableMouseOver();
}

void StatusDisplayButton::enterEvent( QEvent* e ) {
    CommandButton::enterEvent( e );

    if( isEnabled() ) {
        emit mouseEnter( globalX(), globalY() );
    }
}

void StatusDisplayButton::leaveEvent( QEvent* e ) {
    CommandButton::enterEvent( e );

    if ( isEnabled() ) {
        emit mouseLeave( globalX(), globalY() );
    }
}

void StatusDisplayButton::requestMouseEnterSignal() {
    emit mouseEnter( globalX(), globalY() );
}

int StatusDisplayButton::globalX() {
    return mapToGlobal( QPoint( 0, 0 ) ).x();
}

int StatusDisplayButton::globalY() {
    return mapToGlobal( QPoint( 0, 0 ) ).y();
}
