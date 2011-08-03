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
#ifndef SEEKBARUI_H
#define SEEKBARUI_H

// Qt Includes
#include <QtGui/QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QMovie>

// Project Includes
#include "PlayHead.h"

namespace Ui {
    class SeekBarUI;
}

class SeekBarUI : public QWidget {
        Q_OBJECT
    public:
        SeekBarUI( QWidget *parent = 0 );
        ~SeekBarUI();

        void setValue( float ratio );
        void enable( bool enabled );
        void handleBufferingAnimationChangeVisibility( bool visible );

    public slots:
        void dragging( QPoint distance );
        void draggingStart();
        void draggingEnd();
        void draggingTimeout();

    signals:
        void valueChanged( float value );

    protected:
        void changeEvent( QEvent *e );
        void mousePressEvent ( QMouseEvent * event ) ;
        int adjustXPlayHeadPos( int x );
        void showBufferingAnimation();
        void hideBufferingAnimation();

    private:
        Ui::SeekBarUI *_ui;
        QImage _bgImage;
        PlayHead *_playHead;
        bool _dragging;
        QTimer *_timer;
        bool _enabled;
        QMovie *_bufferingAnimation;
};

#endif // SEEKBARUI_H
