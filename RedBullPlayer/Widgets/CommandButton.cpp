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
#include "CommandButton.h"

#include <QPainter>

using namespace RedBullPlayer::Widgets;

CommandButton::CommandButton(
    QWidget *parent,
    QString normalImageUrl,
    int w,
    int h ) : QPushButton( parent ) {
    _images = new QMap<ButtonImageType, QPixmap*>();
    QPixmap pm = QPixmap( normalImageUrl );

    if ( ( w != 0 ) && ( h != 0 ) ) {
        pm = pm.scaled( w,
                        h,
                        Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation );
        _width = w;
        _height = h;
    } else {
        _width = pm.width();
        _height = pm.height();
    }

    _imageSize = QSize( _width, _height );
    this->resize( _imageSize );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    this->setFlat( true );
    _images->insert( RedBullPlayer::Widgets::NORMAL, new QPixmap( pm ) );
    enableMouseOver();
    _isMouseOver = false;
    setCursor( Qt::PointingHandCursor );
    setFocusPolicy( Qt::NoFocus );
}

CommandButton::~CommandButton() {
    if ( !_images->isEmpty() ) {
        if ( _images->contains( RedBullPlayer::Widgets::NORMAL ) ) {
            delete _images->take( RedBullPlayer::Widgets::NORMAL );
        }

        if ( _images->contains( RedBullPlayer::Widgets::MOUSEOVER ) ) {
            delete _images->take( RedBullPlayer::Widgets::MOUSEOVER );
        }

        if ( _images->contains( RedBullPlayer::Widgets::DISABLED ) ) {
            delete _images->take( RedBullPlayer::Widgets::DISABLED );
        }
    }

    delete _images;
}

void CommandButton::setEnabled( bool enabled ) {
    QWidget::setEnabled( enabled );

    if( enabled ) {
        setCursor( Qt::PointingHandCursor );
    } else {
        setCursor( Qt::ArrowCursor );
    }
}

void CommandButton::setMouseOverImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::MOUSEOVER, imageUrl );
}

void CommandButton::setImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::NORMAL, imageUrl );
}

void CommandButton::setDisabledImage( QString imageUrl ) {
    addImage( RedBullPlayer::Widgets::DISABLED, imageUrl );
}

void CommandButton::addImage( ButtonImageType imageType, QString imageUrl ) {
    QPixmap image;

    if ( _images->contains( imageType ) ) {
        delete _images->take( imageType );
    }

    image = QPixmap( imageUrl );
    image = image.scaled( _width,
                          _height,
                          Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation );
    _images->insert( imageType, new QPixmap( image ) );
}

QPixmap CommandButton::getImage( ButtonImageType imageType ) {
    if ( _images->contains( imageType ) ) {
        return *_images->value( imageType );
    } else if ( _images->contains( RedBullPlayer::Widgets::NORMAL ) ) {
        return *_images->value( RedBullPlayer::Widgets::NORMAL ); //use normal as default image
    }

    return 0; //replace with placeholder from imagefactory?
}

void CommandButton::enterEvent( QEvent * ) {
    _isMouseOver = true;

    if ( mouseOverEnabled() && isEnabled() ) {
        update();
    }
}

void CommandButton::leaveEvent( QEvent * ) {
    _isMouseOver = false;

    if ( mouseOverEnabled() && isEnabled() ) {
        update();
    }
}

void CommandButton::paintEvent( QPaintEvent * ) {
    QPainter painter( this );

    if ( !isEnabled() ) {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::DISABLED ) );
    } else if ( _isMouseOver ) {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::MOUSEOVER ) );
    } else {
        painter.drawPixmap( 0, 0, getImage( RedBullPlayer::Widgets::NORMAL ) );
    }
}

QSize CommandButton::sizeHint() const {
    return _imageSize;
}
