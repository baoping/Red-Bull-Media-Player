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
#include "AutoResizeImageLabel.h"

#include <QDebug>

AutoResizeImageLabel::AutoResizeImageLabel( QWidget *parent ) :
    QLabel( parent ) {
    _displayMovie = false;
    _nativeUsed = false;
}

AutoResizeImageLabel::~AutoResizeImageLabel() {
}

void AutoResizeImageLabel::setImage( QString fileName, int frameCount ) {
    _nativeUsed = false;
    _frameCount = frameCount;
    QMovie* movie = new QMovie( fileName );

    if ( movie->isValid() && //image files have FrameCount -1
            ( movie->frameCount() == 0 || movie->frameCount() > 1 ) ) { //PLease Do not Leave out this line of Code Crucial for Pixture animated gif differentiation
        connect( movie, SIGNAL( frameChanged( int ) ), this, SLOT( animationFrameChanged( int ) ) );
        setMovie_internal( movie );
    }  else {
        setPixmap_internal( QPixmap( fileName ) );
    }
}

void AutoResizeImageLabel::setMovie( QMovie* movie, int frameCount ) {
    _nativeUsed = true;
    _frameCount = frameCount;
    QLabel::setMovie( movie );
    connect( movie, SIGNAL( frameChanged( int ) ), this, SLOT( animationFrameChanged( int ) ) );
}

void AutoResizeImageLabel::animationFrameChanged( int frameNumber ) {
    if( frameNumber == _frameCount - 1 )
        this->movie()->setPaused( true );
}

void AutoResizeImageLabel::setPixmap_internal( QPixmap pixmap ) {
    _displayMovie = false;
    _pixmap = pixmap;
    QPixmap p;

    if( _pixmap.height() > height() || _pixmap.width() > width() )
        p = _pixmap.scaled( width(),
                            height(),
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation );
    else
        p = _pixmap;

    QLabel::setPixmap( p );
}

void AutoResizeImageLabel::setMovie_internal( QMovie* movie ) {
    _displayMovie = true;
    _movie = movie;
    QLabel::setMovie( _movie );
    _movie->start();
    QSize size =  _movie->currentPixmap().scaled( width(),
                  height(),
                  Qt::KeepAspectRatio,
                  Qt::SmoothTransformation ).size();
    _movie->setScaledSize( size );
}

void AutoResizeImageLabel::clear() {
    QLabel::clear();
    _pixmap = QPixmap();
    _movie = NULL;
}

void AutoResizeImageLabel::resizeEvent( QResizeEvent * e ) {
    refreshSize();
    QLabel::resizeEvent( e );
}

void AutoResizeImageLabel::refreshSize() {
    if ( !  _nativeUsed ) {
        if ( ! _pixmap.isNull() && ! _displayMovie ) {
            QPixmap p;

            if( _pixmap.height() > height() || _pixmap.width() > width() )
                p = _pixmap.scaled( width(),
                                    height(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation );
            else
                p = _pixmap;

            QLabel::setPixmap( p );
        } else if ( _movie &&
                    _displayMovie &&
                    _movie->isValid() &&
                    ! _movie->currentPixmap().isNull() ) {
            QSize size =  _movie->currentPixmap().scaled( width(),
                          height(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation ).size();
            _movie->setScaledSize( size );
        }
    }
}


