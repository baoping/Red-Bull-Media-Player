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
#ifndef VERSIONSERVICECLIENT_H
#define VERSIONSERVICECLIENT_H

#include <QObject>
#include <QUuid>
#include <QUrl>

// Project Includes
#include "PlayerVersion.h"
#include "../ToolLib/Network/NetworkAccessManager.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace RedBullPlayer {
    namespace Updater {

        class VersionServiceClient : public QObject {
                Q_OBJECT

            signals:
                void versionFound( QString newPlayerVersion, QString downloadUrl, qulonglong zipSize, qulonglong unzipSize );
                void noVersionFound();

            public:
                VersionServiceClient( QUrl serviceUrl, QObject* parent );
                ~VersionServiceClient();

                void checkForUpdate();

            private:
                void parseVersionServiceResponse( QByteArray data );
                void bootstrap();

                QUrl _serviceUrl;
                RedBullPlayer::Tools::Network::NetworkAccessManager* _networkAccessManager;

            private slots:
                void finished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength );
                void failed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase );

        };
    }
}

#endif // VERSIONSERVICECLIENT_H
