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
#ifndef INTERNETCONNECTIONCHECKERMODULE_H
#define INTERNETCONNECTIONCHECKERMODULE_H

// Qt includes
#include <QObject>
#include <QString>
#include <QDomElement>
#include <QEvent>

// Project includes
#include "../../Interfaces/IInternetConnectionChecker.h"
#include "../../PlayerShell/Shell.h"

#include "internetconnectionstatusprovider.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace InternetConnectionChecker {
            class InternetConnectionCheckerModule : public IInternetConnectionChecker {
                Q_OBJECT
                Q_INTERFACES( RedBullPlayer::Modules::InternetConnectionChecker::IInternetConnectionChecker )

            public:
                        InternetConnectionCheckerModule();
                virtual void cleanUp();
                virtual void Setup( QObject* shell, QDomElement config );
                virtual void SetupModuleConnections();
                virtual QString name() const {
                    return "InternetConnectionChecker";
                }
                virtual QString version() const {
                    return "1.0.0";
                }

                virtual bool internetAvailable();


            private:
                RedBullPlayer::PlayerShell::Shell* _shell;
                InternetConnectionStatusProvider* _statusProvider;
                bool _lastState;

            private slots:
                void statusChanged( bool internetAvailable );
            };
        }
    }
}

#endif // INTERNETCONNECTIONCHECKERMODULE_H
