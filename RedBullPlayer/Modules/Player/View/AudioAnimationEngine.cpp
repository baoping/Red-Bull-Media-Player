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
#include "AudioAnimationEngine.h"

#include "math.h"

// Qt Includes
#include <QDebug>

//Project Includes
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Container/Constants.h"

// Qt Animation Framework Includes
# include "../../../../3rdParty/src/qtanimationframework-2.3/src/qtparallelanimationgroup.h"
# include "../../../../3rdParty/src/qtanimationframework-2.3/src/qtpropertyanimation.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

AudioAnimationEngine::AudioAnimationEngine( QWidget* paintSurface, QObject *parent ) : QObject( parent ) {
    _paintSurface = paintSurface;
    _ellipseAnim = 0;
    _figure8Anim = 0;
    _randomAnim = 0;
    _tiledAnim = 0;
    _centeredAnim = 0;
    _mainPixmapAnim = 0;
    _active = false;
    _timer = new QTimer( this );
    _timer->setInterval( 5000 );
    connect( _timer, SIGNAL( timeout() ), this, SLOT( timeoutSlot() ) );
    IMAGE_WIDTH = 75;
    ITEMS_COUNT = 64;
    _lastAnim = -1;
    _mainPixmap = NULL;
}

AudioAnimationEngine::~AudioAnimationEngine() {
    stopAnimations();
}

void AudioAnimationEngine::startAnimation( QPixmap img ) {
    if( _active ) {
        stopAllAnimations();
    }

    _active = true;

    clearItems();
    QPixmap scaledPixmap = img.scaled( IMAGE_WIDTH, IMAGE_WIDTH, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    int centerX = ( _paintSurface->width() - IMAGE_WIDTH ) / 2;
    int centerY = ( _paintSurface->height() - IMAGE_WIDTH ) / 2;

    for ( int i = 0; i < ITEMS_COUNT; i++ ) {
        QLabel *item = new QLabel( _paintSurface );
        item->setStyleSheet( "background-color: transparent" );
        item->setPixmap( scaledPixmap );
        item->resize( IMAGE_WIDTH, IMAGE_WIDTH );
        item->move( centerX, centerY );
        item->show();
        _items << item;
    }

    _mainPixmap = new QLabel( _paintSurface );
    _mainPixmap->setStyleSheet( "background-color: transparent" );
    _mainPixmap->setPixmap( img.scaled( IMAGE_WIDTH * 2, IMAGE_WIDTH * 2, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
    _mainPixmap->resize( IMAGE_WIDTH * 2, IMAGE_WIDTH * 2 );
    _mainPixmap->move( ( _paintSurface->width() - IMAGE_WIDTH * 2 ) / 2, ( _paintSurface->height() - IMAGE_WIDTH * 2 ) / 2 );
    showEllipseAnimation();
    _timer->start();
}

void AudioAnimationEngine::stopAnimations() {
    stopAllAnimations();
    _timer->stop();
    clearItems();
    _active = false;
}

void AudioAnimationEngine::pauseAnimation() {
    if( _timer->isActive() ) {
        _timer->stop();
    }
}

void AudioAnimationEngine::unpauseAnimation() {
    if( !_timer->isActive() ) {
        _timer->start();
    }
}

void AudioAnimationEngine::clearItems() {
    if( _ellipseAnim != 0 )
        _ellipseAnim->clearAnimations();

    if( _centeredAnim != 0 )
        _centeredAnim->clearAnimations();

    if( _tiledAnim != 0 )
        _tiledAnim->clearAnimations();

    if( _randomAnim != 0 )
        _randomAnim->clearAnimations();

    if( _figure8Anim != 0 )
        _figure8Anim->clearAnimations();

    for ( int i = 0; i < _items.count(); i++ ) {
        if( _items.at( i ) != NULL ) {
            _items.at( i )->hide();
            _items.at( i )->deleteLater();
        }
    }

    _items.clear();
}

void AudioAnimationEngine::timeoutSlot() {
    int curr = qrand() % 5;

    if( _lastAnim == curr ) {
        return;
    }

    _lastAnim = curr;

    switch( curr ) {
        case 0:
            showEllipseAnimation();
            break;

        case 1:
            showFigure8Animation();
            break;

        case 2:
            showRandomAnimation();
            break;

        case 3:
            showTiledAnimation();
            break;

        case 4:
            showCenteredAnimation();
            break;
    }
}

void AudioAnimationEngine::addAnimation( QWidget *item, int x, int y, int index, QtParallelAnimationGroup *group ) {
    if( item == NULL )
        return;

    int centerX = ( _paintSurface->width() - IMAGE_WIDTH ) / 2;
    int centerY = ( _paintSurface->height() - IMAGE_WIDTH ) / 2;
    item->show();
    QtPropertyAnimation *anim = new QtPropertyAnimation( item, "pos", group );
    anim->setEndValue( QPointF( x + centerX, y + centerY ) );
    anim->setDuration( 750 + index * 25 );
    anim->setEasingCurve( QtEasingCurve::InOutBack );
    group->addAnimation( anim );
}


void AudioAnimationEngine::showEllipseAnimation() {
    if( _ellipseAnim )
        delete _ellipseAnim;

    _ellipseAnim = new QtParallelAnimationGroup( this );

    for ( int i = 0; i < _items.count(); i++ ) {
        if( _items[i] != NULL )
            addAnimation(
                    _items[i],
                    (int) (cos( ( i / ( float )( ITEMS_COUNT - 1 ) ) * 6.28 ) * 150),
                    (int) (sin( ( i / ( float )( ITEMS_COUNT - 1 ) ) * 6.28 ) * 150),
                    i,
                    _ellipseAnim );
    }

    _mainPixmap->hide();
    _ellipseAnim->start();
}

void AudioAnimationEngine::showFigure8Animation() {
    if( _figure8Anim )
        delete _figure8Anim;

    _figure8Anim = new QtParallelAnimationGroup( this );

    for ( int i = 0; i < _items.count(); i++ ) {
        addAnimation(
                _items[i],
                (int) ( sin( ( i / ( float ) ( ITEMS_COUNT - 1 ) ) * 6.28 ) * 150 ),
                (int) ( sin( ( ( i * 2 ) / ( float )( ITEMS_COUNT - 1 ) ) * 6.28 ) * 150 ),
                i,
                _figure8Anim );
    }

    if ( _mainPixmap == 0 ) {
        //qError() << "AudioAnimationEngine::MainPixmap is null";
    }

    _mainPixmap->hide();
    _figure8Anim->start();
}

void AudioAnimationEngine::showRandomAnimation() {
    if( _randomAnim )
        delete _randomAnim;

    _randomAnim = new QtParallelAnimationGroup( this );

    for ( int i = 0; i < _items.count(); i++ ) {
        addAnimation( _items[i], -150 + qrand() % 300, -150 + qrand() % 300, i, _randomAnim );
    }

    _mainPixmap->hide();
    _randomAnim->start();
}

void AudioAnimationEngine::showTiledAnimation() {
    if( _tiledAnim )
        delete _tiledAnim;

    _tiledAnim = new QtParallelAnimationGroup( this );

    for ( int i = 0; i < _items.count(); i++ ) {
        addAnimation( _items[i], ( ( i % 8 ) - 4 ) * _items[i]->width() + _items[i]->width() / 2,
                      ( ( i / 8 ) - 4 ) * _items[i]->height() + _items[i]->height() / 2, i, _tiledAnim );
    }

    _mainPixmap->hide();
    _tiledAnim->start();
}

void   AudioAnimationEngine::showCenteredAnimation() {
    if( _centeredAnim )
        delete _centeredAnim;

    _centeredAnim = new QtParallelAnimationGroup( this );
    connect( _centeredAnim, SIGNAL( finished() ), this, SLOT( centeredAnimationFinished() ) );

    for ( int i = 0; i < _items.count(); i++ ) {
        addAnimation( _items[i], 0, 0, i, _centeredAnim );
    }

    _mainPixmap->hide();
    _centeredAnim->start();
}

void AudioAnimationEngine::centeredAnimationFinished() {
    if( _mainPixmapAnim )
        delete _mainPixmapAnim;

    for ( int i = 0; i < _items.count(); i++ ) {
        _items[i]->hide();
    }

    _mainPixmapAnim = new QtPropertyAnimation( _mainPixmap, "size", this );
    _mainPixmapAnim->setStartValue( QSize( 0, 0 ) );
    _mainPixmapAnim->setEndValue( QSize( IMAGE_WIDTH * 2, IMAGE_WIDTH * 2 ) );
    _mainPixmapAnim->setDuration( 1000 );
    _mainPixmapAnim->setEasingCurve( QtEasingCurve::InOutBack );
    _mainPixmap->show();
    _mainPixmapAnim->start();
}

void AudioAnimationEngine::stopAllAnimations() {
    stopAnimation( _ellipseAnim );
    stopAnimation( _figure8Anim );
    stopAnimation( _randomAnim );
    stopAnimation( _tiledAnim );
    stopAnimation( _centeredAnim );

    if( _mainPixmap != NULL )
        if( _mainPixmap->isVisible() )
            _mainPixmap->hide();
}

void AudioAnimationEngine::stopAnimation( QtParallelAnimationGroup *anim ) {
    if( anim != NULL ) {
        anim->stop();
    }
}
