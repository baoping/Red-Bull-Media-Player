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
#include "ToogleButton.h"

#include <QPainter>
#include <QFile>

using namespace RedBullPlayer::Widgets;

ToogleButton::ToogleButton( QWidget *parent,
                            QString normalImageUrl,
                            QString checkedImageUrl ) : QToolButton( parent ), ImageListHandler() {
    addImage( RedBullPlayer::Widgets::NORMAL, normalImageUrl );
    addImage( RedBullPlayer::Widgets::CHECKED, checkedImageUrl );
    this->resize( _imageSize );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setCheckable( true );
    enableMouseOver();
    _isMouseOver = false;
    setCursor( Qt::PointingHandCursor );
    setFocusPolicy( Qt::NoFocus );
}

ToogleButton::~ToogleButton() {
}

void ToogleButton::setEnabled( bool enabled ) {
    QWidget::setEnabled( enabled );

    if( enabled ) {
        setCursor( Qt::PointingHandCursor );
    } else {
        setCursor( Qt::ArrowCursor );
    }
}

void ToogleButton::setMouseOverImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::MOUSEOVER, imageUrl );
}

void ToogleButton::setMouseOverImageForToggledHover( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::MOUSEOVER, imageUrl );
    _isMouseOver = false;
}

void ToogleButton::setImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::NORMAL, imageUrl );
}

void ToogleButton::setDisabledImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::DISABLED, imageUrl );
}

void ToogleButton::setCheckedImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::CHECKED, imageUrl );
}

void ToogleButton::enterEvent( QEvent * ) {
    _isMouseOver = true;

    if ( mouseOverEnabled() && isEnabled() ) {
        update();
    }
}

void ToogleButton::leaveEvent( QEvent * ) {
    _isMouseOver = false;

    if ( mouseOverEnabled() && isEnabled() ) {
        update();
    }
}

void ToogleButton::paintEvent( QPaintEvent * ) {
    QPainter painter( this );

    if ( !isEnabled() ) {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::DISABLED ) );
    } else if ( _isMouseOver ) {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::MOUSEOVER ) );
    } else if ( isChecked() ) {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::CHECKED ) );
    } else {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::NORMAL ) );
    }
}

QSize ToogleButton::sizeHint() const {
    return _imageSize;
}

