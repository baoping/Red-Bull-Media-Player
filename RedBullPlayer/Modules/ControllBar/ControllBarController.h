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
#ifndef CONTROLLBARCONTROLLER_H
#define CONTROLLBARCONTROLLER_H

#include <QObject>
#include <QTimer>

//MVC
#include "ControllBarModel.h"
#include "ControllBarUI.h"

//Events
#include "ControllBarEventBridge.h"

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../PlayerShell/Shell.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ControllBar {
            class ControllBarController : public QObject {
                    Q_OBJECT

                public:
                    explicit ControllBarController( RedBullPlayer::PlayerShell::Shell* shell,
                                                    RedBullPlayer::PlayerShell::Region* region,
                                                    ControllBarModel* model,
                                                    ControllBarUI* view,
                                                    QObject *parent = 0 );
                    ~ControllBarController();

                private:
                    ControllBarEventBridge* createControllBarEventBridge( bool connectSignals = true );
                    void updateViewPosition( int parentRegionWidth, int parentRegionHeight );
                    void initRegion();
                    void setVisible( bool visible );

                    ControllBarModel* _model;
                    ControllBarUI* _view;

                    ControllBarEventBridge* _controllBarEventBridge;

                    bool _isPlaying;
                    bool _isVisible;
                    bool _isBuffering;

                    QTimer* _tvBufferingTimer; //time display during tv stream buffering
                    int _tvBufferingTime;


                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    QTimer* _hideTimer;

                    int _lastParentWitdh;
                    int _lastParentHeight;

                    bool _skipDocuments;

                    bool _isPlayerFullScreen;

                private slots:
                    //Model
                    void onMediaChanged();

                    //MediaPlayer
                    void updateControllBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media, bool skipDocuments );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    void playerVisibilityChanged( bool visible );
                    void snapshotCaptureEnabled( bool enable );
                    void playerForwardSpeedChanged( int speed );
                    void playerBackwardSpeedChanged( int speed );
                    void playerVolumneChanged( int volumne );

                    //TVBuffering
                    void onTvBufferingTimer();

                    //CentralUI
                    void centralUIRegionResized( int width, int height );
                    void shellWidgetMoved();

                    //Hide/Show Timer
                    void onHideTimer();

                    //PlayerView
                    void videoScreenMouseMoved();

                    //View
                    void onViewAction();
                    void onBackwardClicked();
                    void onPlayClicked();
                    void onForwardClicked();
                    void onPreviewClicked();
                    void onFullScreenClicked();
                    void onSnapshotClicked();
                    void onMuteClicked();
                    void onResolutionClicked();

                    //Common
                    void keyPressed( QKeyEvent* keyEvent );
                    void fullScreenStatusChanged( bool isFullScreen );

                    //Buffering
                    void bufferingAnimationChangedVisibility( bool visible );

                    // Player Exit
                    void playerExitClicked();
            };
        }
    }
}

#endif // CONTROLLBARCONTROLLER_H
