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
#ifndef CONTROLLBAREVENTBRIDGE_H
#define CONTROLLBAREVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Events/ControllBar/ControllBarBackwardButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarForwardButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarFullscreenButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPauseButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPlayButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarPreviewButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarSnapshotButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarMuteButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarResolutionButtonClickedEvent.h"

#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {

            class ControllBarEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit ControllBarEventBridge( QObject *parent = 0 );
                    ~ControllBarEventBridge();

                    void updateControllBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                protected:
                    bool event( QEvent* event );

                    void onControllBarPauseButtonClicked( RedBullPlayer::Events::ControllBarPauseButtonClickedEvent *e );
                    void onControllBarPlayButtonClicked( RedBullPlayer::Events::ControllBarPlayButtonClickedEvent *e );
                    void onControllBarSnapshotButtonClicked( RedBullPlayer::Events::ControllBarSnapshotButtonClickedEvent *e );
                    void onControllBarFullscreenButtonClicked( RedBullPlayer::Events::ControllBarFullscreenButtonClickedEvent *e );
                    void onControllBarMuteButtonClicked( RedBullPlayer::Events::ControllBarMuteButtonClickedEvent *e );
                    void onControllBarForwardButtonClicked( RedBullPlayer::Events::ControllBarForwardButtonClickedEvent *e );
                    void onControllBarBackwardButtonClicked( RedBullPlayer::Events::ControllBarBackwardButtonClickedEvent *e );
                    void onControllBarResolutionButtonClicked( RedBullPlayer::Events::ControllBarResolutionButtonClickedEvent *e );

                signals:
                    void pauseClicked();
                    void playClicked();
                    void snapShotClicked();
                    void fullScreenClicked();
                    void muteClicked( bool muted );
                    void forwardClicked();
                    void backwardClicked();
                    void resolutionClicked();

            };
        }
    }
}

#endif // CONTROLLBAREVENTBRIDGE_H
