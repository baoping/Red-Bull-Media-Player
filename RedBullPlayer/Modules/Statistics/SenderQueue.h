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
#ifndef SENDERQUEUE_H
#define SENDERQUEUE_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QString>

#include "../../Interfaces/Statistics/IStatistics.h"
#include "../../Interfaces/MediaLibrary/IStatisticClientCacheRepository.h"
#include "../../Interfaces/MediaLibrary/IStatisticEntry.h"
#include "../../ToolLib/Network/NetworkAccessManager.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace Statistics {
            class SenderQueue : public QObject {
                    Q_OBJECT
                public:
                    SenderQueue( RedBullPlayer::Modules::Statistics::IStatistics* statisticsModule, QObject *parent = 0 );

                    bool queueEntry( RedBullPlayer::Modules::MediaLibrary::IStatisticEntry* entry );

                    bool internetAvailable() const {
                        return _internetAvailable;
                    }

                    QQueue<RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*> queue() const {
                        return _queue;
                    }

                    bool timerActive() const {
                        return _timer->isActive();
                    }

                    QTimer* timer() const {
                        return _timer;
                    }

                    QUrl generateRequestUrl( const RedBullPlayer::Modules::MediaLibrary::IStatisticEntry& entry );

                    RedBullPlayer::Tools::Network::NetworkAccessManager* networkAccessManager() const {
                        return _networkAccessManager;
                    }

                    RedBullPlayer::Modules::MediaLibrary::IStatisticEntry* currentEntry() const {
                        return _currentEntry;
                    }

                    void setInternetAvailable( bool available );
                    void setPlayerUuid( QUuid playerUuid );
                    void setApiKey( QString apiKey );


                signals:
                    void entrySentToServer( QString entryUuid );
                    void entrySentToServerFailed( QString entryUuid );

                public slots:
                    void timerShot();
                    void requestFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength );
                    void requestFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase );

                private slots:


                private:
                    RedBullPlayer::Modules::Statistics::IStatistics* _statisticsModule;
                    RedBullPlayer::Modules::MediaLibrary::IStatisticEntry* _currentEntry;
                    QQueue<RedBullPlayer::Modules::MediaLibrary::IStatisticEntry*> _queue;
                    QTimer* _timer;
                    QUuid _playerUuid;
                    QString _apiKey;
                    bool _internetAvailable;
                    RedBullPlayer::Tools::Network::NetworkAccessManager* _networkAccessManager;
            };
        }
    }
}

#endif // SENDERQUEUE_H
