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
#ifndef ISYSTEMMETADATAREPOSITORY_H
#define ISYSTEMMETADATAREPOSITORY_H

// Qt includes
#include <QObject>
#include <QString>
#include <QUuid>

#define PLAYER_UUID_METADATA_DATAKEY (QString)"PlayerUUID"
#define PLAYER_ALLOW_COLLECT_STATISTIC_DATAKEY (QString)"AllowCollectStatistic"
#define PLAYER_ALLOW_NETWORK_CONNECTION_DATAKEY (QString)"AllowNetworkConnection"
#define PLAYER_SHOW_USER_SETTINGS_DIALOG_DATAKEY (QString)"ShowUserSettingsDialog"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class ISystemMetaDataRepository  {
                public:
                    virtual ~ISystemMetaDataRepository() {}
                    virtual QString getMetaData( const QString& dataKey ) = 0;
                    virtual QString getDBVersion() = 0;
                    virtual QUuid getPlayerUUID() = 0;

                    virtual bool allowCollectStatistic() = 0;
                    virtual void setAllowCollectStatistic( bool allowCollectStatistic ) = 0;

                    virtual bool allowNetworkConnection() = 0;
                    virtual void setAllowNetworkConnection( bool alllowNetworkConnection ) = 0;

                    virtual bool getShowUserSettingsDialog() = 0;
                    virtual void setShowUserSettingsDialog( bool showUserSettingsDialog ) = 0;
            };
        }
    }
}

#endif // ISYSTEMMETADATAREPOSITORY_H
