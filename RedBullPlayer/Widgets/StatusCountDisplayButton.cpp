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
#include "StatusCountDisplayButton.h"

#include <QFontMetrics>

using namespace RedBullPlayer::Widgets;

StatusCountDisplayButton::StatusCountDisplayButton( int commandButtonWidth, int commandButtonHeight,
        QFont font, QColor borderColorCircle, qreal borderThickness,
        QColor backgroundColorCircle, QColor textColor,
        QString normalImageUrl, QString disabledImageUrl, QWidget *parent )
    : StatusDisplayButton( normalImageUrl, disabledImageUrl, parent, commandButtonWidth, commandButtonHeight ) {
    _normalImage = normalImageUrl;
    _disabledImage = disabledImageUrl;
    _font = font;
    _borderColorCircle = borderColorCircle;
    _borderThickness = borderThickness;
    _backgroundColorCircle = backgroundColorCircle;
    _textColor = textColor;
    _count = "";
    this->resize( QSize( commandButtonWidth, commandButtonHeight ) );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setCursor( Qt::PointingHandCursor );
    setFocusPolicy( Qt::NoFocus );
}

void StatusCountDisplayButton::setNewCount( int count ) {
    if( count == 0 )
        _count = "";
    else
        _count = QString::number( count );

    update();
}

void StatusCountDisplayButton::paintEvent( QPaintEvent * ) {
    QPainter painter( this );

    if ( !isEnabled() ) {
        painter.drawPixmap( 0, 0, _disabledImage );
    } else {
        painter.drawPixmap( 0, 0, _normalImage );

        if( !_count.isEmpty() )
            drawCountGraphic( &painter );
    }
}

void StatusCountDisplayButton::drawCountGraphic( QPainter *painter ) {
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->setFont( _font );
    int radius =  0;
    int textHeight = painter->fontMetrics().height();
    int textWidth = painter->fontMetrics().width( _count );

    if( textHeight > textWidth )
        radius = (int) ( 0.7 * textHeight );
    else
        radius = (int) ( 0.7 * textWidth );

    painter->setPen( QPen( _borderColorCircle, _borderThickness ) );
    painter->setBrush( QBrush( _backgroundColorCircle ) );
    painter->drawEllipse( QPoint( (int) (  _normalImage.width() + 0.6 * radius ),
                                 (int) ( _normalImage.height() + 0.6 * radius ) ),
                          radius, radius );
    painter->setPen( QPen( QBrush( _textColor ), 1.0 ) );
    painter->drawText( (int) ( _normalImage.width() - 0.4 * radius ) ,
                       (int) ( _normalImage.height() - 0.4 * radius ),
                       2 * radius,
                       2 * radius,
                       Qt::AlignCenter | Qt::AlignVCenter, _count );
}
