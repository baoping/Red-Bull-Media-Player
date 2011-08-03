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
#ifndef PLAYERCLOSEBUTTONMODULE_H
#define PLAYERCLOSEBUTTONMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/IPlayerCloseButton.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/Player/PlayerVisibilityChangedEvent.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../Events/ShellWidgetMovedEvent.h"
#include "../../Events/ControllBar/ControllBarVisibilityChangedEvent.h"
#include "PlayerCloseButtonUI.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace PlayerCloseButton {
            class PlayerCloseButtonModule : public IPlayerCloseButton {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::PlayerCloseButton::IPlayerCloseButton )

                public:
                    PlayerCloseButtonModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "PlayerCloseButton";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                public slots:
                    void exitClickedSlot();
                    void back2PlaylistClickedSlot();

                protected:
                    void initRegionWidget();
                    bool event ( QEvent * e );
                    void onPlayerVisibilityChanged( RedBullPlayer::Events::PlayerVisibilityChangedEvent* e );
                    void onCentralUIRegionResized( RedBullPlayer::Events::CentralUIRegionResizedEvent *e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent *e );
                    void onControllBarVisibilityChanged( RedBullPlayer::Events::ControllBarVisibilityChangedEvent *e );
                    void updatePosition();

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    int _centralUIRegionWidth;
                    int _centralUIRegionHeight;

                    PlayerCloseButtonUI *_ui;
                    bool _adjustPosOnNextTimeTick;
                    int DISTANCE_BETWEEN_BUTTONS;
                    bool _isFullScreen;
            };
        }
    }
}

#endif // PLAYERCLOSEBUTTONMODULE_H
