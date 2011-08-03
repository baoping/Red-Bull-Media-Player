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
#include "PlayerView.h"
#include "ui_PlayerView.h"

#include <QDebug>

PlayerView::PlayerView( QWidget *parent ) :
    RedBullPlayer::Modules::Player::IPlayerView( parent ),
    ui( new Ui::PlayerView ) {
    ui->setupUi( this );
    setCurrentWidget( ui->dummySurface );
    _mouseMoveTimer = new QTimer( this );
    _mouseMoveTimer->start( 300 );
    connect( _mouseMoveTimer, SIGNAL( timeout() ), this, SLOT( onMouseMoveTimer() ) );
    _applicationActive = true;
    _mouseWasInPlayerView = false;
    _lastMouseMovePoint = QPoint( 0, 0 );
    _bufferingAnimation = 0;
    _currentFrameCount = 0;
}

PlayerView::~PlayerView() {
    delete ui;
}

void PlayerView::changeEvent( QEvent *e ) {
    QStackedWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

bool PlayerView::event( QEvent* e ) {
    if ( e->type() == QEvent::WindowActivate ) {
        _applicationActive = true;
    }

    if( e->type() == QEvent::WindowDeactivate ) {
        _applicationActive = false;
    }

    return false;
}

void PlayerView::showVideoPage() {
    setCurrentWidget( ui->videoSurface );
}

void PlayerView::showImagePage() {
    setCurrentWidget( ui->imageSurface );
}

void PlayerView::showAudioPage() {
    setCurrentWidget( ui->audioSurface );
}

void PlayerView::showDummyPage() {
    setCurrentWidget( ui->dummySurface );
}

QWidget* PlayerView::videoPage() const {
    return ui->videoSurface;
}

QWidget* PlayerView::audioPage() const {
    return ui->audioSurface;
}

QWidget* PlayerView::imagePage() const {
    return ui->imageSurface;
}

bool PlayerView::isVideoPageDisplayed() {
    return ( currentIndex() == indexOf( ui->videoSurface ) );
}

void PlayerView::clearImage() {
    ui->imageContainer->clear();
}

void PlayerView::setImage( QString fileName ) {
    clearImage();

    if( fileName.isEmpty() ) return;

    ui->imageContainer->setImage( fileName );
}

void PlayerView::onMouseMoveTimer() {
    if( !isVisible() || !_applicationActive ) return;

    QPoint mouseGlobalPos = QCursor::pos();
    QPoint topLeft = mapToGlobal( QPoint( 0, 0 ) );
    QPoint bottomRight = mapToGlobal( QPoint( width(), height() ) );
    bool mouseIsInPlayerView = QRect( topLeft, bottomRight ).contains( mouseGlobalPos );

    if( _mouseWasInPlayerView != mouseIsInPlayerView && mouseIsInPlayerView ) {
        emit mouseMovedOnPlayerView();
    } else {
        if( mouseIsInPlayerView && _lastMouseMovePoint != mouseGlobalPos ) {
            emit mouseMovedOnPlayerView();
        }
    }

    _mouseWasInPlayerView = mouseIsInPlayerView;
    _lastMouseMovePoint = mouseGlobalPos;
}

void PlayerView::setBufferingAnimation( QString bufferingAnimationFile, int frameCount ) {
    ui->imageContainer->setImage( bufferingAnimationFile, frameCount );
}

