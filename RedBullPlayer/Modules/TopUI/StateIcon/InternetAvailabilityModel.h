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
#ifndef INTERNETAVAILABILITYMODEL_H
#define INTERNETAVAILABILITYMODEL_H

#include "StateIconModel.h"
#include "../../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class InternetAvailabilityModel : public StateIconModel {
                    Q_OBJECT
                public:
                    InternetAvailabilityModel( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition,
                                               RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary, QObject *parent = 0 );

                protected:
                    bool event( QEvent *event );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent *event );

                signals:
                    void internetConnectionStatusChanged( bool internetAvailable );

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
            };
        }
    }
}

#endif // INTERNETAVAILABILITYMODEL_H
