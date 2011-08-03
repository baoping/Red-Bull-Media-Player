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
#ifndef INTERNETAVAILABILITYPRESENTEREVENTBRIDGE_H
#define INTERNETAVAILABILITYPRESENTEREVENTBRIDGE_H

#include <QObject>

#include "StatePresenterEventBridge.h"
#include "../../Interfaces/StateDisplay/IInternetAvailabilityPresenter.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class InternetAvailabilityPresenterEventBridge : public StatePresenterEventBridge {
                    Q_OBJECT
                public:
                    InternetAvailabilityPresenterEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose,
                            IInternetAvailabilityPresenter *internetAvailabilityPresenter,
                            QObject *parent = 0 );
                protected:
                    bool event( QEvent* event );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent *event );

                private:
                    IInternetAvailabilityPresenter* _internetAvailabilityPresenter;
                    RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum _purpose;
            };
        }
    }
}

#endif // INTERNETAVAILABILITYPRESENTEREVENTBRIDGE_H
