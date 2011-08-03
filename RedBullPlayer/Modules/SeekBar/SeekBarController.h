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
#ifndef SEEKBARCONTROLLER_H
#define SEEKBARCONTROLLER_H

#include <QObject>
#include <QTimer>

//MVC
#include "SeekBarModel.h"
#include "SeekBarUI.h"

//Events
#include "SeekBarEventBridge.h"

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SeekBar {

            class SeekBarController : public QObject {
                    Q_OBJECT
                public:
                    explicit SeekBarController( SeekBarModel* model, SeekBarUI* view, QObject *parent = 0 );
                    ~SeekBarController();

                private: //data
                    SeekBarModel* _model;
                    SeekBarUI* _view;

                    SeekBarEventBridge* _seekBarEventBridge;

                private: //functions
                    SeekBarEventBridge* createSeekBarEventBridge( bool connectSignals = true );
                private slots:
                    //Model
                    void onMediaChanged();
                    //MediaPlayer
                    void updateSeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    //Buffering
                    void bufferingAnimationChangedVisibility( bool visible );
                    //View
                    void valueChanged( float value );


            };
        }
    }
}

#endif // SEEKBARCONTROLLER_H
