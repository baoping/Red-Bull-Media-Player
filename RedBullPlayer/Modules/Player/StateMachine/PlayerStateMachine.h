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
#ifndef PLAYERSTATEMACHINE_H
#define PLAYERSTATEMACHINE_H

#include <QObject>
#include <QHash>

//Project includes
#include "IPlayerState.h"
#include "../../../Interfaces/Enumerations/MediaPlayerStateEnum.h"
#include "../View/IPlayerView.h"
#include "../PlayerModel.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace Player {

            class PlayerStateMachine : public QObject {
                    Q_OBJECT
                public:
                    PlayerStateMachine( PlayerModel* playerModel,
                                        MediaPlayerHandler* mediaPlayerHandler,
                                        IPlayerView* view,
                                        AudioAnimationEngine* audioAnimationEngine,
                                        QObject *parent = 0 );
                    ~PlayerStateMachine();

                    bool changeStateTo( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );

                private: //functions
                    void bootstrap();


                private: //data
                    MediaPlayerHandler* _mediaPlayerHandler;
                    PlayerModel* _playerModel;
                    IPlayerView* _view;
                    AudioAnimationEngine* _audioAnimationEngine;

                    RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum _currentState;

                    QHash<RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum, IPlayerState*> _states;
            };
        }
    }
}

#endif // PLAYERSTATEMACHINE_H
