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
#include "HelpWidget.h"

#include <QPainter>
#include <QHelpEvent>
#include <QApplication>
#include <QToolTip>

using namespace RedBullPlayer::Widgets;

HelpWidget::HelpWidget( QWidget *parent, QString normalImageUrl, QString hoverImageUrl, QString helpText ) : QWidget( parent ) {
    _image = QPixmap( normalImageUrl );
    _hoverImage = QPixmap( hoverImageUrl );
    _helpText = helpText;
    _currentImage = _image;
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; }" );
    setToolTip( helpText );
    this->setFixedSize( _image.width(), _image.height() );
}

void HelpWidget::enterEvent( QEvent *e ) {
    _currentImage = _hoverImage;
    update();
    sendToolTipRequest();
}

void HelpWidget::leaveEvent( QEvent *e ) {
    _currentImage = _image;
    update();
    QToolTip::hideText();
}

void HelpWidget::paintEvent( QPaintEvent * ) {
    QPainter painter( this );
    painter.drawPixmap( 0, 0, _currentImage );
}

void HelpWidget::sendToolTipRequest() {
    QHelpEvent *event = new QHelpEvent( QEvent::ToolTip,
                                        QPoint( this->pos().x(), this->pos().y() ),
                                        QPoint( QCursor::pos().x(), QCursor::pos().y() ) );
    QApplication::postEvent( this, event );
}
