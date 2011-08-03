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
#ifndef STATEDISPLAYMODULE_H
#define STATEDISPLAYMODULE_H

//QT Includes
#include <QObject>
#include <QMap>

// Project Includes
#include "../../Interfaces/StateDisplay/IStateDisplay.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/StateDisplay/IStateViewFactory.h"
#include "../../Interfaces/StateDisplay/ITaskViewFactory.h"
#include "../../Interfaces/StateDisplay/ITaskPresenter.h"
#include "../../Interfaces/StateDisplay/ITaskModel.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/StateDisplay/IDiskSpaceModel.h"
#include "../../Interfaces/StateDisplay/IDiskSpacePresenter.h"
#include "../../Interfaces/StateDisplay/IInternetAvailabilityPresenter.h"
#include "../../Interfaces/StateDisplay/IInternetAvailabilityModel.h"
#include "taskviewmodeleventbridge.h"
#include "taskpresentereventbridge.h"
#include "InternetAvailabilityPresenterEventBridge.h"
#include "StatePresenterEventBridge.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class StateDisplayModule : public IStateDisplay {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::StateDisplay::IStateDisplay )

                public:
                    StateDisplayModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "StateDisplay";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                protected:
                    bool event( QEvent * e );
                    void resolveStyleFactory();
                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    IStateViewFactory* _stateViewFactory;
                    ITaskViewFactory* _taskViewFactory;

                    ITaskPresenter* _downloadPresenter;
                    ITaskPresenter* _ioPresenter;
                    IDiskSpacePresenter* _diskSpacePresenter;
                    ITaskPresenter* _updatePresenter;
                    IInternetAvailabilityPresenter*_internetAvailabilityPresenter;

                    ITaskModel* _downloadModel;
                    ITaskModel* _ioModel;
                    IDiskSpaceModel* _diskSpaceModel;
                    ITaskModel* _updateModel;
                    IInternetAvailabilityModel *_internetAvailabilityModel;

                    TaskViewModelEventBridge* _downloadModelEventBridge;
                    TaskViewModelEventBridge* _ioModelEventBridge;
                    TaskViewModelEventBridge* _updateModelEventBridge;

                    TaskPresenterEventBridge* _downloadPresenterEventBridge;
                    TaskPresenterEventBridge* _ioPresenterEventBridge;
                    TaskPresenterEventBridge* _updatePresenterEventBridge;

                    StatePresenterEventBridge* _diskSpaceEventBridge;
                    InternetAvailabilityPresenterEventBridge *_internetAvailabilityEventBridge;

                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;

                    void bootstrap();
            };
        }
    }
}

#endif // STATEDISPLAYMODULE_H
