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
#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>

#include "../MediaPlayer/MediaPlayerHandler.h"
#include "../View/IPlayerView.h"
#include "../PlayerModel.h"
#include "../View/AudioAnimationEngine.h"
#include "../../../Container/Constants.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class IPlayerState : public QObject {
                    Q_OBJECT

                public:
                    IPlayerState( PlayerModel* playerModel,
                                  MediaPlayerHandler* mediaPlayerHandler,
                                  IPlayerView* view,
                                  AudioAnimationEngine* audioAnimationEngine,
                                  QObject* parent = 0 ) : QObject( parent ) {
                        _playerModel = playerModel;
                        _mediaPlayerHandler = mediaPlayerHandler;
                        _view = view;
                        _audioAnimationEngine = audioAnimationEngine;
                    }
                    virtual ~IPlayerState() {}

                    virtual bool enterState() = 0;
                    virtual bool leaveState() = 0;

                protected: //data
                    MediaPlayerHandler* mediaPlayerHandler() {
                        return _mediaPlayerHandler;
                    }
                    PlayerModel* playerModel() {
                        return _playerModel;
                    }
                    IPlayerView* view() {
                        return _view;
                    }
                    AudioAnimationEngine* audioAnimationEngine() {
                        return _audioAnimationEngine;
                    }

                private: //data
                    PlayerModel* _playerModel;
                    MediaPlayerHandler* _mediaPlayerHandler;
                    IPlayerView* _view;
                    AudioAnimationEngine* _audioAnimationEngine;
            };
        }
    }
}

#endif // IPLAYERSTATE_H
