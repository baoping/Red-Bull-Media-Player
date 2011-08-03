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
#ifndef MEDIAPLAYERSTATEENUM_H
#define MEDIAPLAYERSTATEENUM_H

#include <QMetaType>

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {

            class MediaPlayerState : QObject {
                    Q_OBJECT
                    Q_ENUMS( StateEnum )

                public:
                    enum StateEnum {
                        NONE = 0,
                        OPENING = 1,
                        BUFFERING = 2,
                        PLAYING = 3,
                        PAUSED = 4,
                        STOPPED = 5,
                        ENDED = 6,
                        ERROR = 7
                    };

                    explicit MediaPlayerState( StateEnum state ) : QObject( 0 ) {
                        _state = state;
                    }

                    StateEnum state() const {
                        return _state;
                    }

                    static QString stateString( StateEnum state ) {
                        QString stateStr = "";

                        switch( state ) {
                            case MediaPlayerState::NONE:
                                stateStr = "NONE";
                                break;

                            case MediaPlayerState::OPENING:
                                stateStr = "OPENING";
                                break;

                            case MediaPlayerState::BUFFERING:
                                stateStr = "BUFFERING";
                                break;

                            case MediaPlayerState::PLAYING:
                                stateStr = "PLAYING";
                                break;

                            case MediaPlayerState::PAUSED:
                                stateStr = "PAUSED";
                                break;

                            case MediaPlayerState::STOPPED:
                                stateStr = "STOPPED";
                                break;

                            case MediaPlayerState::ENDED:
                                stateStr = "ENDED";
                                break;

                            case MediaPlayerState::ERROR:
                                stateStr = "ERROR";
                                break;
                        }

                        return stateStr;
                    }
                private:
                    StateEnum _state;
            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum );



#endif // MEDIAPLAYERSTATEENUM_H
