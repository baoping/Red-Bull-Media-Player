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
#include "MirrorWidget.h"

//QT Includes
#include <QtGui>

//Projects Include
#include "../Container/Constants.h"


using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;

MirrorWidget::MirrorWidget( QWidget *parent, const QString &imgUrl, const QString &activeImgUrl, const QString &darkImgUrl, const QString &darkActiveImgUrl, const QString &title, bool hideAtStart, int id /*=-1*/ )
    : ClickableLabel( parent, id ) {
    _img = QImage( imgUrl );
    _activeImg = QImage( activeImgUrl );
    _darkActiveImg = QImage( darkActiveImgUrl );
    _darkImg = QImage( darkImgUrl );
    _currentImg = &_img;
    _scaleFactor = 1;
    _title = title;
    _img = _img.convertToFormat( QImage::Format_ARGB32_Premultiplied );
    _hasMainUIRegionFocus = false;
    _active = false;
    setStyleSheet( "background-color : transparent;" );

    if( hideAtStart ) {
        hide();
    }
}


QSize MirrorWidget::sizeHint() const {
    return QSize( _img.width(), _img.height() * 2 );
}


void MirrorWidget::paintEvent( QPaintEvent */*e*/ ) {
    QPainter p( this );
    QImage scaled = _currentImg->scaled( ( int ) ( _scaleFactor * _img.width() ), ( int ) ( _scaleFactor * _img.height() ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    p.drawImage( 0, 0, scaled );
    QImage mirroredImg = MirrorWidget::mirrorImage( scaled, 50, MirrorWidget::MirrorOverX, MirrorWidget::NoFade );
    p.drawImage( 0, scaled.height(), mirroredImg );
}

qreal MirrorWidget::scaleFactor() {
    return _scaleFactor;
}

void MirrorWidget::setScaleFactor( qreal newValue ) {
    _scaleFactor = newValue;
    update();
}

QString MirrorWidget::title() {
    return _title;
}


void MirrorWidget::scaleToSize( QSize newSize ) {
    _img = _img.scaled( newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    _activeImg = _activeImg.scaled( newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    _darkImg = _darkImg.scaled( newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    _darkActiveImg = _darkActiveImg.scaled( newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

    if( _active ) {
        if( _hasMainUIRegionFocus )
            _currentImg = &_activeImg;
        else
            _currentImg = &_darkActiveImg;
    } else {
        if( _hasMainUIRegionFocus )
            _currentImg = &_img;
        else
            _currentImg = &_darkImg;
    }
}

QImage MirrorWidget::mirrorImage( const QImage &img, int alpha, MirrorStyle mirrorStyle/*=MirrorOverX*/, FadeStyle fadeStyle/*=FadeDown*/ ) {
    //alpha does not work ad is not used  Bug fix black reflection on some images
    if ( fadeStyle != NoFade ) {
        QPoint p1, p2;

        if ( fadeStyle == FadeDown )
            p2.setY( img.height() );
        else if ( fadeStyle == FadeUp )
            p1.setY( img.height() );
        else if ( fadeStyle == FadeRight )
            p2.setX( img.width() );
        else if ( fadeStyle == FadeLeft )
            p1.setX( img.width() );

        QLinearGradient gradient( p1, p2 );
        gradient.setColorAt( 0, QColor( 125, 125, 125 ) );
        gradient.setColorAt( 0.4, QColor( 65, 65, 65 ) );
        gradient.setColorAt( 0.85, QColor( 0, 0, 0 ) );
        gradient.setColorAt( 1, QColor( 0, 0, 0 ) );
        QImage mask = img;
        QPainter p( &mask );
        p.fillRect( img.rect(), gradient );
        p.end();
        QImage reflection = img.mirrored( mirrorStyle == MirrorOverY,
                                          mirrorStyle == MirrorOverX );
        reflection.setAlphaChannel( mask );
        return reflection;
    } else {
        QImage mask = img;
        QPainter p( &mask );
        p.fillRect( img.rect(), QBrush( QColor( 125, 125, 125 ) ) );
        p.end();
        QImage reflection = img.mirrored( mirrorStyle == MirrorOverY,
                                          mirrorStyle == MirrorOverX );
        reflection.setAlphaChannel( mask );
        return reflection;
    }
}

void MirrorWidget::setActive( bool active ) {
    _active = active;

    if( !active ) {
        if( _hasMainUIRegionFocus && isEnabled() )
            _currentImg = &_img;
        else
            _currentImg = &_darkImg;
    } else {
        if( _hasMainUIRegionFocus && isEnabled() )
            _currentImg = &_activeImg;
        else
            _currentImg = &_darkActiveImg;
    }

    update();
}

bool MirrorWidget::hasMainRegionFocus() {
    return _hasMainUIRegionFocus;
}

void MirrorWidget::setMainRegionFocus( bool hasFocus ) {
    _hasMainUIRegionFocus = hasFocus;
    /*if(_hasMainUIRegionFocus)
    {
        if(_active)
            _currentImg = &_activeImg;
        else
            _currentImg = &_img;
    }
    else
    {
        if(_active)
            _currentImg = &_darkActiveImg;
        else
            _currentImg = &_darkImg;
    }*/
    setActive( _active );
}

void MirrorWidget::setEnabled( bool enabled ) {
    ClickableLabel::setEnabled( enabled );
    setActive( _active );
}

bool MirrorWidget::isActive() {
    return _active;
}


