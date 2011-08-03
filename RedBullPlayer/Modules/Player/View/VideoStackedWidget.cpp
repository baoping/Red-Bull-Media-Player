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
#include "VideoStackedWidget.h"
#include "ui_VideoStackedWidget.h"

#include <QDebug>

#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../../EventAggregator/Aggregator.h"
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Container/Constants.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

VideoStackedWidget::VideoStackedWidget( QWidget *parent, Shell *shell ) :
    QStackedWidget( parent ),
    _ui( new Ui::VideoStackedWidget ) {
    _ui->setupUi( this );
    _shell = shell;
    _mouseMoveTimer = new QTimer( this );
    _mouseMoveTimer->start( 300 );
    connect( _mouseMoveTimer, SIGNAL( timeout() ), this, SLOT( mouseMoveTimerSlot() ) );
    _lastIsIn = false;
    _lastMovePoint = QPoint( 0, 0 );
    _isAnimationSet = false;
    _isApplicationWindowActive = true;
    _isBufferingAnimationSet = false;
}

VideoStackedWidget::~VideoStackedWidget() {
    delete _ui;
}

QWidget* VideoStackedWidget::videoSurface() {
    return _ui->_videoOutput;
}

void VideoStackedWidget::mouseMoveTimerSlot() {
    if( !isVisible() || !_isApplicationWindowActive ) {
        return;
    }

    QPoint mouseGlobalPos = QCursor::pos();
    QPoint topLeft = mapToGlobal( QPoint( 0, 0 ) );
    QPoint bottomRight = mapToGlobal( QPoint( width(), height() ) );
    bool isIn = QRect( topLeft, bottomRight ).contains( mouseGlobalPos );

    if( _lastIsIn != isIn ) {
        emitMouseMoveAndSendFocusChanged();
    } else {
        if( isIn && _lastMovePoint != mouseGlobalPos ) {
            emitMouseMoveAndSendFocusChanged();
        }
    }

    _lastIsIn = isIn;
    _lastMovePoint = mouseGlobalPos;
}

void VideoStackedWidget::emitMouseMoveAndSendFocusChanged() {
    emit mouseMove();

    if( isVisible() ) {
        MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
        Aggregator::instance()->sendEvent( &e );
    }
}

void VideoStackedWidget::mousePressEvent( QMouseEvent* event ) {
    emit mousePressed();
}

void VideoStackedWidget::mouseDoubleClickEvent( QMouseEvent */*event*/ ) {
    emit mouseDoubleClicked();
}

void VideoStackedWidget::mouseReleaseEvent ( QMouseEvent */*event*/ ) {
    emit mouseReleased();
}

void VideoStackedWidget::enterEvent( QEvent *e ) {
    if( isVisible() ) {
        MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
        Aggregator::instance()->sendEvent( &e );
    }
}

void VideoStackedWidget::changeEvent( QEvent *e ) {
    QStackedWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void VideoStackedWidget::setImageContentType( QString imageUrl ) {
    if( imageUrl.isEmpty() )
        return;

    QMovie *animatedImage = new QMovie( imageUrl );
    _origPixmap = QPixmap( imageUrl );

    if( animatedImage->isValid() && ( animatedImage->frameCount() == 0 || animatedImage->frameCount() > 1 ) )
        setAnimatedImage( animatedImage, false );
    else
        setPixmap( _origPixmap, false );
}

void VideoStackedWidget::setPixmap( QPixmap pixmap, bool newPixmap ) {
    _ui->_imageViewer->clear();

    if( newPixmap ) _origPixmap = pixmap;

    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( _origPixmap.width() > playlistRegion->width() || _origPixmap.height() > playlistRegion->height() ) {
        pixmap = _origPixmap.scaled( playlistRegion->width(), playlistRegion->height(), Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation );
    }

    _ui->_imageViewer->setFixedSize( pixmap.size() );
    centerImageViewer();
    _ui->_imageViewer->setPixmap( pixmap );
    _isAnimationSet = false;
}

void VideoStackedWidget::setAnimatedImage( QMovie *animation, bool isBufferingAnimation ) {
    if( animation == NULL )
        return;

    if( _origPixmap.isNull() )
        return;

    _isAnimationSet = !isBufferingAnimation;
    _isBufferingAnimationSet = isBufferingAnimation;
    _ui->_imageViewer->clear();
    _ui->_imageViewer->setMovie( animation );

    if( _isAnimationSet )
        resizeAnimation();
    else if( _isBufferingAnimationSet )
        resizeBufferingAnimation();

    animation->start();
}

void VideoStackedWidget::setBufferingAnimation( QString animationUrl, int frameCount ) {
    if( animationUrl.isEmpty() )
        return;

    QMovie *animatedImage = new QMovie( animationUrl );
    _origPixmap = QPixmap( animationUrl );

    if( animatedImage->isValid() ) {
        _currentBufferingAnimation = animatedImage;
        _currentBufferingAnimationFrameCount = frameCount;
        connect( _currentBufferingAnimation, SIGNAL( frameChanged( int ) ), this, SLOT( animationFrameChangedSlot( int ) ) );
        setAnimatedImage( animatedImage, true );
    }
}

void VideoStackedWidget::animationFrameChangedSlot( int frameNumber ) {
    if( _currentBufferingAnimation == NULL )
        return;

    if( frameNumber == _currentBufferingAnimationFrameCount - 1 )
        _currentBufferingAnimation->setPaused( true );
}

void VideoStackedWidget::resizeEvent( QResizeEvent *event ) {
    if( _origPixmap.isNull() )
        return;

    if( currentIndex() == 1 ) {
        if( _isAnimationSet )
            resizeAnimation();
        else if( _isBufferingAnimationSet )
            resizeBufferingAnimation();
        else
            resizePixmap();
    }
}

void VideoStackedWidget::resizePixmap() {
    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( _origPixmap.width() > playlistRegion->width() || _origPixmap.height() > playlistRegion->height() ) {
        QPixmap pixmap = _origPixmap.scaled( playlistRegion->width(), playlistRegion->height(), Qt::KeepAspectRatio,
                                             Qt::FastTransformation );
        _ui->_imageViewer->setFixedSize( pixmap.size() );
    }

    centerImageViewer();
}

void VideoStackedWidget::resizeAnimation() {
    if( _ui->_imageViewer->movie() == NULL )
        return;

    if( _origPixmap.isNull() )
        return;

    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( _origPixmap.width() > playlistRegion->width() || _origPixmap.height() > playlistRegion->height() )
        resizeAnimationKeepAspectRatio( playlistRegion );
    else
        _ui->_imageViewer->setFixedSize( _origPixmap.size() );

    centerImageViewer();
}

void VideoStackedWidget::resizeBufferingAnimation() {
    if( _ui->_imageViewer->movie() == NULL )
        return;

    if( _origPixmap.isNull() )
        return;

    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();
    QPixmap scaledPixmap;
    scaledPixmap = _origPixmap.scaled( playlistRegion->size(), Qt::KeepAspectRatio, Qt::FastTransformation );
    _ui->_imageViewer->setFixedSize( scaledPixmap.size() );
    centerImageViewer();
}

void VideoStackedWidget::resizeAnimationKeepAspectRatio( QWidget *containerWidget ) {
    if( _ui->_imageViewer->movie() == NULL )
        return;

    QPixmap scaledPixmap = _origPixmap.scaled( containerWidget->size(), Qt::KeepAspectRatio, Qt::FastTransformation );
    _ui->_imageViewer->setFixedSize( scaledPixmap.size() );
}

void VideoStackedWidget::centerImageViewer() {
    int x = ( int )( ( _ui->_imageOutput->width() - _ui->_imageViewer->width() ) / 2 );
    int y = ( int )( ( _ui->_imageOutput->height() - _ui->_imageViewer->height() ) / 2 );
    _ui->_imageViewer->move( x, y );
}

void VideoStackedWidget::repaintEmptyAreaAroundImageViewer() {
    int imageViewerHeight = _ui->_imageViewer->height();
    int imageOutputWidth = _ui->_imageOutput->width();
    int widthDiffrence = ( int )( ( imageOutputWidth - _ui->_imageViewer->width() ) / 2 );
    int yBottom = ( _ui->_imageViewer->pos().y() +  imageViewerHeight );
    int xRight = ( _ui->_imageViewer->pos().x() + _ui->_imageViewer->width() );
    int yImageViewer = _ui->_imageViewer->pos().y();
    _ui->_imageOutput->repaint( 0, 0, imageOutputWidth, _ui->_imageViewer->pos().y() );
    _ui->_imageOutput->repaint( 0, yBottom, imageOutputWidth, _ui->_imageOutput->height() - yBottom );
    _ui->_imageOutput->repaint( 0, yImageViewer, widthDiffrence, imageViewerHeight );
    _ui->_imageOutput->repaint( xRight, yImageViewer, widthDiffrence, imageViewerHeight );
}

bool VideoStackedWidget::event( QEvent *e ) {
    if ( e->type() == QEvent::WindowActivate ) {
        _isApplicationWindowActive = true;
    }

    if( e->type() == QEvent::WindowDeactivate ) {
        _isApplicationWindowActive = false;
    }

    return QStackedWidget::event( e );
}

