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
#include "SeekBarUI.h"
#include "ui_SeekBarUI.h"

// Qt Includes
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <math.h>

// Project Includes
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "PlayHead.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

SeekBarUI::SeekBarUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::SeekBarUI ),
    _bgImage( Constants::SEEK_BAR_IMAGE_DIR + Constants::SEEK_BAR_BG_BORDER ) {
    _ui->setupUi( this );
    _playHead = new PlayHead( this );
    _playHead->move( 2, 3 );
    _dragging = false;
    _timer = new QTimer( this );
    _timer->setSingleShot( true );
    _enabled = true;
    _bufferingAnimation = new QMovie( Constants::SEEK_BAR_IMAGE_DIR + Constants::SEEK_BAR_BUFFERING_ANIMATION );
    _ui->_bufferingAnimationLabel->hide();
    connect( _playHead, SIGNAL( dragging( QPoint ) ), this, SLOT( dragging( QPoint ) ) );
    connect( _playHead, SIGNAL( draggingStart() ), this, SLOT( draggingStart() ) );
    connect( _playHead, SIGNAL( draggingEnd() ), this, SLOT( draggingEnd() ) );
    connect( _timer, SIGNAL( timeout() ), this, SLOT( draggingTimeout() ) );
    _ui->_seekbarContainer->setStyleSheet( QString( "background-image: url(%1); background-repeat: none; background-position:top left;" ).arg( Constants::SEEK_BAR_IMAGE_DIR + Constants::SEEK_BAR_BG_BORDER ) );
}

SeekBarUI::~SeekBarUI() {
    delete _ui;
}

void SeekBarUI::mousePressEvent ( QMouseEvent * event ) {
    if( _enabled ) {
        float ratio = 0.0;

        if( event->pos().x() < ( _playHead->width() / 2 ) )
            ratio = 0.0;
        else if( event->pos().x() > ( width() - ( ( _playHead->width() / 2 ) + 1 ) ) )
            ratio = 1.0;
        else
            ratio = ( float )( event->pos().x() - ( ( _playHead->width() / 2 ) + 1 ) )  / ( float )( width() - 2 - _playHead->width() );

        emit valueChanged( ratio );
    }
}

void SeekBarUI::setValue( float ratio ) {
    if( ratio < 0 || ratio > 1 || _dragging ) {
        return;
    }

    int fullWidth = width() - 2 - _playHead->width();
    int newX = ( int )( ( fullWidth  * ratio ) ) + 1;
    _playHead->move( newX, _playHead->y() );
}

void SeekBarUI::dragging( QPoint distance ) {
    _timer->stop();
    QPoint newP = distance + _playHead->pos();
    int newX = adjustXPlayHeadPos( newP.x() );
    _playHead->move( newX, _playHead->y() );
    _timer->start( Constants::SEEK_BAR_DRAGGING_TIMEOUT );
}

void SeekBarUI::draggingStart() {
    _dragging = true;
}

void SeekBarUI::draggingEnd() {
    _dragging = false;
    draggingTimeout();
}

void SeekBarUI::draggingTimeout() {
    _timer->stop();
    int fullWidth = width() - 2 - _playHead->width();
    float ratio = ( float ) ( _playHead->x() ) / ( float ) fullWidth;
    emit valueChanged( ratio );
}

int SeekBarUI::adjustXPlayHeadPos( int x ) {
    if( x > width() - _playHead->width() - 1 ) {
        x = width() - _playHead->width() - 1;
    }

    if( x < 1 ) {
        x = 1;
    }

    return x;
}

void SeekBarUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SeekBarUI::enable( bool enabled ) {
    _enabled = enabled;

    if( _enabled ) {
        _playHead->show();
        setCursor( Qt::PointingHandCursor );
    } else {
        _playHead->hide();
        setCursor( Qt::ArrowCursor );
    }
}

void SeekBarUI::handleBufferingAnimationChangeVisibility( bool visible ) {
    if( visible )
        showBufferingAnimation();
    else
        hideBufferingAnimation();
}

void SeekBarUI::showBufferingAnimation() {
    if( _bufferingAnimation == NULL )
        return;

    _ui->_bufferingAnimationLabel->setMovie( _bufferingAnimation );
    _ui->_bufferingAnimationLabel->show();
    _bufferingAnimation->start();
}

void SeekBarUI::hideBufferingAnimation() {
    _ui->_bufferingAnimationLabel->clear();
    _ui->_bufferingAnimationLabel->hide();
}
