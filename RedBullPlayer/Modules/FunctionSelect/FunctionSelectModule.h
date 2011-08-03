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
#ifndef FUNCTIONSELECTMODULE_H
#define FUNCTIONSELECTMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "FunctionSelectUI.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/IFunctionSelect.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace FunctionSelect {
            class FunctionSelectModule : public IFunctionSelect {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::FunctionSelect::IFunctionSelect )

                public:
                    FunctionSelectModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "FunctionSelectModule";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                public slots:
                    void functionChangedSlot( QString newFunctionTitle );

                protected:
                    bool event( QEvent *event );
                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent *event );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent *event );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    FunctionSelectUI *_ui;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
            };
        }
    }
}

#endif // FUNCTIONSELECTMODULE_H
