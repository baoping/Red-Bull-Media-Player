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
#ifndef VIDEOSCREENSTACKEDWIDGET_H
#define VIDEOSCREENSTACKEDWIDGET_H

#include <QtGui/QStackedWidget>
#include <QTimer>
#include <QMovie>

#include "../../../PlayerShell/Shell.h"

namespace Ui {
    class VideoStackedWidget;
}

class VideoStackedWidget : public QStackedWidget {
        Q_OBJECT
    public:
        VideoStackedWidget( QWidget *parent, RedBullPlayer::PlayerShell::Shell *shell );
        ~VideoStackedWidget();

        void setImageContentType( QString imageUrl );
        void setBufferingAnimation( QString animationUrl, int frameCount );

        QWidget* videoSurface();

    signals:
        void mouseMove();
        void mouseDoubleClicked();
        void mouseReleased();
        void mousePressed();

    public slots:
        void mouseMoveTimerSlot();
        void animationFrameChangedSlot( int frameNumber );

    protected:
        void changeEvent( QEvent *e );
        void mousePressEvent( QMouseEvent* event );
        void mouseDoubleClickEvent ( QMouseEvent *event );
        void mouseReleaseEvent ( QMouseEvent *event );
        void resizeEvent( QResizeEvent *e );
        void enterEvent( QEvent *e );
        void setPixmap( QPixmap pixmap, bool newPixmap = true );
        void setAnimatedImage( QMovie *animation, bool isBufferingAnimation );
        void resizeAnimation();
        void resizeAnimationKeepAspectRatio( QWidget *containerWidget );
        void resizePixmap();
        void resizeBufferingAnimation();
        void centerImageViewer();
        void repaintEmptyAreaAroundImageViewer();

        bool event( QEvent *e );

    private: //data
        Ui::VideoStackedWidget *_ui;
        QTimer *_mouseMoveTimer;
        bool _lastIsIn;
        QPoint _lastMovePoint;
        RedBullPlayer::PlayerShell::Shell *_shell;
        QPixmap _origPixmap;
        bool _isAnimationSet;
        QMovie *_currentBufferingAnimation;
        int _currentBufferingAnimationFrameCount;
        bool _isApplicationWindowActive;
        bool _isBufferingAnimationSet;

    private: //functsion
        void emitMouseMoveAndSendFocusChanged();
};

#endif // VIDEOSCREENSTACKEDWIDGET_H
