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
#ifndef CENTRALUIMODULE_H
#define CENTRALUIMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/ICentralUI.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/CentralUIWidgetChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/Player/PlayerVisibilityChangedEvent.h"
#include "CentralUICreator.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace CentralUI {
            class CentralUIModule : public ICentralUI {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::CentralUI::ICentralUI )

                public:
                    CentralUIModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "CentralUI";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    virtual bool event ( QEvent * e );
                    void setCurrentWidget( RedBullPlayer::Events::CentralUIWidgetChangedEvent::CentralUIWidgets widget );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void emitCentralFocusChangedEvent( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
                    void onCentralFocusedChanged( RedBullPlayer::Events::CentralFocusChangedEvent *e );
                    void onPlayerVisibilityChanged( RedBullPlayer::Events::PlayerVisibilityChangedEvent *e );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    CentralUICreator *_ui;
            };
        }
    }
}

#endif // CENTRALUIMODULE_H
