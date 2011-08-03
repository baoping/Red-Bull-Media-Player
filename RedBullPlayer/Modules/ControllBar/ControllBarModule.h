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
#ifndef CONTROLLBARMODULE_H
#define CONTROLLBARMODULE_H

//QT Includes
#include <QObject>
#include <QTimer>

// Project Includes
#include "../../Interfaces/IControllBar.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../PlayerShell/Shell.h"

#include "ControllBarModel.h"
#include "ControllBarController.h"
#include "ControllBarUI.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace ControllBar {
            class ControllBarModule : public IControllBar {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::ControllBar::IControllBar )

                public:
                    ControllBarModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual void cleanUp();
                    virtual QString name() const {
                        return "ControllBar";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    //MVC
                    ControllBarModel* _model;
                    ControllBarController* _controller;
                    ControllBarUI* _view;

                    //Project
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* _styleFactory;

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* resolveMediaLibrary();
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* resolveStyleFactory();
            };
        }
    }
}

#endif // CONTROLLBARMODULE_H
