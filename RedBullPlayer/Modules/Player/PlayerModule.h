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
#ifndef PLAYERMODULE_H
#define PLAYERMODULE_H

//QT Includes
#include <QObject>
#include <QTimer>

// Project Includes
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/Player/IPlayer.h"
#include "../../Interfaces/IPlaylistItem.h"
#include "../../PlayerShell/Shell.h"

#include "View/AudioAnimationEngine.h"
#include "View/VideoStackedWidget.h"
#include "View/IPlayerView.h"
#include "MediaPlayer/MediaPlayerHandler.h"
#include "PlayerModel.h"
#include "PlayerController.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class PlayerModule : public IPlayer {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::Player::IPlayer )

                public:
                    PlayerModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual void cleanUp();
                    virtual QString name() const {
                        return "Player";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                private: //data
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    MediaPlayerHandler* _mediaPlayerHandler;

                    VideoStackedWidget *_vout;
                    AudioAnimationEngine *_audioAnimEngine;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    SnapshotHelper* _snapshotHelper;

                    //MVC
                    PlayerModel* _model;
                    PlayerController* _controller;
                    IPlayerView* _view;

                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* _styleFactory;

                private: //functions
                    void resolveMediaLibrary();
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* resolveStyleFactory();

            };
        }
    }
}

#endif // PLAYERMODULE_H
