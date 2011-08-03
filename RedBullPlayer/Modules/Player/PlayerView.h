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
#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QStackedWidget>
#include <QPixmap>
#include <QMovie>
#include <QTimer>
#include <QPoint>
#include <QSize>
#include <QResizeEvent>

//Project includes
#include "View/IPlayerView.h"

namespace Ui {
    class PlayerView;
}

class PlayerView : public RedBullPlayer::Modules::Player::IPlayerView {
        Q_OBJECT
    public:
        PlayerView( QWidget *parent = 0 );
        virtual ~PlayerView();

        virtual void showVideoPage();
        virtual void showImagePage();
        virtual void showAudioPage();
        virtual void showDummyPage();

        virtual void setImage( QString fileName );
        virtual void clearImage();

        virtual void setBufferingAnimation( QString bufferingAnimationFile, int frameCount );

        virtual QWidget* videoPage() const;
        virtual QWidget* audioPage() const;
        virtual QWidget* imagePage() const;

        virtual bool isVideoPageDisplayed();

    protected:
        void changeEvent( QEvent *e );
        bool event( QEvent* e );

    private: //data
        Ui::PlayerView *ui;

        QTimer* _mouseMoveTimer;

        bool _applicationActive;
        bool _mouseWasInPlayerView;
        QPoint _lastMouseMovePoint;

        QMovie* _bufferingAnimation;
        int _currentFrameCount;

    private slots:
        void onMouseMoveTimer();

};

#endif // PLAYERVIEW_H
