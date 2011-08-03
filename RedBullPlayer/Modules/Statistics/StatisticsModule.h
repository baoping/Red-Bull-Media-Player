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
#ifndef STATISTICSMODULE_H
#define STATISTICSMODULE_H

//QT Includes
#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QPixmap>

// Project Includes
#include "../../Interfaces/Statistics/IStatistics.h"
#include "../../Interfaces/MediaLibrary/IStatisticClientCacheRepository.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/Statistics/CreateStatisticEntryEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"

#include "SenderQueue.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Statistics {
            class StatisticsModule : public IStatistics {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::Statistics::IStatistics )

                public:
                    StatisticsModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "StateDisplay";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                    bool internetAvailable() const {
                        return _internetAvailable;
                    }

                    RedBullPlayer::Modules::MediaLibrary::IStatisticClientCacheRepository* statisticClientCacheRepository() const {
                        return _statisticClientCacheRepository;
                    }

                    RedBullPlayer::Modules::MediaLibrary::ISystemMetaDataRepository* systemMetaDataRepository() const {
                        return _systemMetaDataRepository;
                    }

                    SenderQueue* senderQueue() const {
                        return _senderQueue;
                    }

                    QUuid playerUuid() const {
                        return _playerUuid;
                    }

                    void setStatisticClientCacheRepository( RedBullPlayer::Modules::MediaLibrary::IStatisticClientCacheRepository* );
                    void setSystemMetaDataRepository( RedBullPlayer::Modules::MediaLibrary::ISystemMetaDataRepository* );
                    void onCreateStatisticsEntry( RedBullPlayer::Events::CreateStatisticEntryEvent* ev );
                    void onInternetStatusChange( RedBullPlayer::Events::InternetConnectionStatusChangedEvent* ev );

                    void queueCachedRequests();

                    void resolveMediaLibrary();

                protected:
                    bool event( QEvent * e );

                private slots:
                    void statisticRequestFinished( QString entryUuid );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    bool _internetAvailable;
                    RedBullPlayer::Modules::MediaLibrary::IStatisticClientCacheRepository* _statisticClientCacheRepository;
                    RedBullPlayer::Modules::MediaLibrary::ISystemMetaDataRepository* _systemMetaDataRepository;
                    SenderQueue* _senderQueue;
                    QUuid _playerUuid;

            };
        }
    }
}

#endif // STATISTICSMODULE_H
