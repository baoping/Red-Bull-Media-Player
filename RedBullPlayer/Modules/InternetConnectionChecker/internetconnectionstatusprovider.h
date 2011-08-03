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
#ifndef INTERNETCONNECTIONSTATUSPROVIDER_H
#define INTERNETCONNECTIONSTATUSPROVIDER_H

#include <QObject>
#include <QString>
#include <QTimer>

//we are not using our own NetworkAccessManager because of unwanted overhead
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "../../ToolLib/Network/InternetStatusChecker.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace InternetConnectionChecker {
            class InternetConnectionStatusProvider : public QObject {
                Q_OBJECT

            public:
                InternetConnectionStatusProvider( QString referenceHost, int checkPeriod, QObject *parent = 0 );
                ~InternetConnectionStatusProvider();
                QString referenceHost() const {
                    return _referenceHost;
                }
                int checkPeriod() const {
                    return _checkPeriod;
                }

                void start();
                void stop();

                bool isRunning();

            signals:
                void statusChanged( bool internetAvailable );

            private: //functions
                void bootstrap();
                void doCheck();

            private slots:
                void onTimer();

            private: //data
                RedBullPlayer::Tools::Network::InternetStatusChecker *_connectionChecker;
                QString _referenceHost;
                int _checkPeriod;
                bool _lastStatus;
                bool _firstTimeCheck;
                QTimer* _checkTimer;
            };
        }
    }
}

#endif // INTERNETCONNECTIONSTATUSPROVIDER_H
