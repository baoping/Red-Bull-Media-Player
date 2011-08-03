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

#include <QObject>

#include "../../Interfaces/StateDisplay/IInternetAvailabilityModel.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class InternetAvailabilityModel : public IInternetAvailabilityModel {
                    Q_OBJECT

                public:
                    InternetAvailabilityModel( QObject* parent = 0 );

                    virtual ~InternetAvailabilityModel();

                    virtual QString onlineText();
                    virtual QString offlineText();

                private:
                    QString _onlineText;
                    QString _offlineText;
            };
        }
    }
}

#endif // INTERNETAVAILABILITYMODEL_H
