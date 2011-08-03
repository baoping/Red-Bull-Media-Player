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
#include "Button.h"

//Qt Includes
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

using namespace RedBullPlayer::Widgets;

Button::Button( QWidget *parent,
                QString normalImgUrl,
                QString onMouseDownImgUrl,
                QString pressedImgUrl,
                QString disabledImgUrl/* = ""*/,
                bool enableMouseOver/* = true*/,
                QString mouseOverImgUrl/* = ""*/ ) : ClickableLabel( parent ) {
    _currentState = Button::NORMAL;
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    _normalIcon = NULL;
    _onMouseDownIcon = NULL;
    _pressedIcon = NULL;
    _disabledIcon = NULL;
    _mouseOverIcon = NULL;
    _enableMouseOver = enableMouseOver;

    if( disabledImgUrl.isEmpty() ) {
        disabledImgUrl = normalImgUrl;
    }

    if( mouseOverImgUrl.isEmpty() ) {
        mouseOverImgUrl = onMouseDownImgUrl;
    }

    setIcons( normalImgUrl,
              onMouseDownImgUrl,
              pressedImgUrl,
              disabledImgUrl,
              mouseOverImgUrl );
    _checked = false;
    _clickDownDistance = 1;
    //setMouseTracking(true); //enable mouse tracking
}

Button::~Button() {
    deleteIcons();
}

QSize Button::sizeHint() const {
    return QSize( _normalIcon->width(), _normalIcon->height()/*+_clickDownDistance*/ );
}

void Button::paintEvent( QPaintEvent */*event*/ ) {
    QPainter painter( this );
    painter.drawPixmap( 0, 0, *_currentIcon );
}

void Button::mousePressEvent ( QMouseEvent * event ) {
    QPoint mousePos = event->pos();

    if( mousePos.x() > 0 && mousePos.x() < width() && mousePos.y() > 0 && mousePos.y() < height() ) {
        setState( Button::ONMOUSEDOWN );
    }
}

void Button::enterEvent( QEvent * ) {
    if ( /*!_checked &&*/ _enableMouseOver && isEnabled() && ( state() != Button::ONMOUSEDOWN ) ) {
        setState( Button::ONMOUSEENTER );
    }
}

void Button::leaveEvent( QEvent * ) {
    if ( /*!_checked &&*/ _enableMouseOver && isEnabled() && ( state() != Button::ONMOUSEDOWN ) ) {
        if( _checked )
            setState( Button::PRESSED );
        else
            setState( Button::ONMOUSELEAVE );
    }
}

void Button::mouseReleaseEvent ( QMouseEvent * event ) {
    QPoint mousePos = event->pos();

    if( mousePos.x() > 0 && mousePos.x() < width() && mousePos.y() > 0 && mousePos.y() < height() ) {
        if( !_checked ) {
            setState( Button::PRESSED );
        } else {
            setState( Button::NORMAL );
        }

        ClickableLabel::mouseReleaseEvent( event );
    }
}

void Button::setIcons(
    QString normalImgUrl,
    QString onMouseDownImgUrl,
    QString pressedImgUrl,
    QString disabledImgUrl,
    QString mouseOverImgUrl/* = ""*/ ) {
    deleteIcons();
    _normalIcon = new QPixmap( normalImgUrl );
    _onMouseDownIcon = new QPixmap( onMouseDownImgUrl );
    _pressedIcon = new QPixmap( pressedImgUrl );
    _disabledIcon = new QPixmap( disabledImgUrl );

    if ( !mouseOverImgUrl.isEmpty() ) {
        _mouseOverIcon = new QPixmap( mouseOverImgUrl );
    } else {
        _mouseOverIcon = new QPixmap( onMouseDownImgUrl );
    }

    if( isEnabled() ) {
        _currentIcon = _normalIcon;
    } else {
        _currentIcon = _disabledIcon;
    }

    resize( _normalIcon->width(), _normalIcon->height() );
    update();
}

void Button::deleteIcons() {
    if( _normalIcon != NULL ) {
        delete _normalIcon;
        _normalIcon = NULL;
    }

    if( _onMouseDownIcon != NULL ) {
        delete _onMouseDownIcon;
        _onMouseDownIcon = NULL;
    }

    if( _pressedIcon != NULL ) {
        delete _pressedIcon;
        _pressedIcon = NULL;
    }

    if( _disabledIcon != NULL ) {
        delete _disabledIcon;
        _disabledIcon = NULL;
    }

    if ( _mouseOverIcon != NULL ) {
        delete _mouseOverIcon;
        _mouseOverIcon = NULL;
    }

    _currentIcon = NULL;
}

void Button::setState( State newState ) {
    if( !isEnabled() ) {
        return;
    }

    _currentState = newState;

    switch( _currentState ) {
        case Button::NORMAL:
            _currentIcon = _normalIcon;
            _checked = false;
            break;

        case Button::ONMOUSEDOWN:
            _currentIcon = _onMouseDownIcon;
            break;

        case Button::ONMOUSEENTER:
            _currentIcon = _mouseOverIcon;
            break;

        case Button::ONMOUSELEAVE:
            _currentIcon = _normalIcon;
            break;

        case Button::PRESSED:
            _currentIcon = _pressedIcon;
            _checked = true;
            break;
    }

    update();
}

Button::State Button::state() {
    return _currentState;
}

void Button::setClickDownDistance( int distance ) {
    _clickDownDistance = distance;
}

void Button::setEnabled( bool enabled ) {
    ClickableLabel::setEnabled( enabled );

    if( enabled ) {
        _currentIcon = _normalIcon;
    } else {
        _currentIcon = _disabledIcon;
    }
}

