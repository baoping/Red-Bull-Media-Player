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
/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */
#ifndef VLC_ON_QT_H
#define VLC_ON_QT_H

#include <vlc/vlc.h>

#include <QWidget>

class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

#define POSITION_RESOLUTION 10000

class Player : public QWidget {
        Q_OBJECT
        QSlider *_positionSlider;
        QSlider *_volumeSlider;
        QFrame *_videoWidget;
        QTimer *poller;
        bool _isPlaying;
        libvlc_exception_t _vlcexcep;
        libvlc_instance_t *_vlcinstance;
        libvlc_media_player_t *_mp;
        libvlc_media_t *_m;

    public:
        Player();
        virtual ~Player();
        void raise( libvlc_exception_t * ex );

    public slots:
        void playFile( QString file );
        void updateInterface();
        void changeVolume( int newVolume );
        void changePosition( int newPosition );

};
#endif
