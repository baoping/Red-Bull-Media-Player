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
#include "CarrouselItem.h"

// Qt Includes
#include <QPainter>
#include <QToolTip>
#include <QString>

// Project Includes
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

CarrouselItem::CarrouselItem( QImage img, int width, int height, QWidget *parent, qint64 chapterStartDurationSeconds, int videoLengthSeconds, int id /*= -1*/ ) : ClickableLabel( parent, id ) {
    _frameWidth = 0;
    _highlighted = false;
    _frameWidth = Constants::CHAPTER_SELECT_ITEMS_MARGIN;
    _height = height;
    _width = width;
    setMinimumWidth( width );
    setMinimumHeight( height );
    _img = QPixmap::fromImage( img, 0 );
    _img = _img.scaled( _width - ( 2 * _frameWidth ), _height - ( 2 * _frameWidth ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; }" );
    setChapterStartToolTip( chapterStartDurationSeconds, videoLengthSeconds );
}

CarrouselItem::~CarrouselItem() {}

void CarrouselItem::setChapterStartToolTip( qint64 chapterStartDurationSeconds, int videoDurationSeconds ) {
    if( chapterStartDurationSeconds < 0 )
        return;

    QString tooltip = "";

    if( videoDurationSeconds < 60 ) {
        tooltip = QString::number( chapterStartDurationSeconds ) + "s";
    } else {
        if( videoDurationSeconds < 3600 ) {
            if( chapterStartDurationSeconds < 60 ) {
                tooltip = "0m " + QString::number( chapterStartDurationSeconds ) + "s";
            } else {
                tooltip = QString::number( ( int )( chapterStartDurationSeconds / 60 ) ) + "m " +
                          QString::number( chapterStartDurationSeconds % 60 ) + "s";
            }
        } else {
            if( chapterStartDurationSeconds < 60 ) {
                tooltip = "0h 0m " + QString::number( chapterStartDurationSeconds ) + "s";
            } else {
                if( chapterStartDurationSeconds < 3600 ) {
                    tooltip = "0h " +
                              QString::number( ( int )( chapterStartDurationSeconds / 60 ) ) + "m " +
                              QString::number( chapterStartDurationSeconds % 60 ) + "s";
                } else {
                    tooltip =  QString::number( ( int )( chapterStartDurationSeconds / 3600 ) ) + "h " +
                               QString::number( ( int )( ( chapterStartDurationSeconds % 3600 ) / 60 ) ) + "m " +
                               QString::number( chapterStartDurationSeconds % 60 ) + "s";
                }
            }
        }
    }

    setToolTip( tooltip );
}

void CarrouselItem::paintEvent( QPaintEvent * /*event*/ ) {
    QPainter painter( this );

    if( _highlighted ) {
        QBrush b( Constants::CHAPTER_SELECT_ACTIVE_ITEM_BG_COLOR );
        painter.setBrush( b );
        painter.drawRect( 0, 0, width(), height() );
    }

    painter.drawPixmap( _frameWidth, _frameWidth, _img );
}

void CarrouselItem::setFrameWidth( int width ) {
    _frameWidth = width;
}

void CarrouselItem::setHighlighted( bool highlighted ) {
    _highlighted = highlighted;
}

bool CarrouselItem::highLighted() {
    return _highlighted;
}

void CarrouselItem::changeChapterImage( QImage img, bool immediateUpdate ) {
    _img = QPixmap::fromImage( img, 0 );
    _img = _img.scaled( _width - ( 2 * _frameWidth ), _height - ( 2 * _frameWidth ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    if( immediateUpdate )
        update();
}
