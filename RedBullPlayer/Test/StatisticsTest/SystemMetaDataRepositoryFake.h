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
#ifndef SYSTEMMETADATAREPOSITORYFAKE_H
#define SYSTEMMETADATAREPOSITORYFAKE_H

// Qt includes
#include <QObject>
#include <QString>
#include <QMutex>
#include <QUuid>
#include <QHash>

// Project includes
#include "MediaDB.h"

#include "../../Container/IError.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            class SystemMetaDataRepositoryFake  :  public QObject, public ISystemMetaDataRepository {
                    Q_OBJECT
                public:
                    SystemMetaDataRepositoryFake( QObject *parent );
                    virtual ~SystemMetaDataRepositoryFake();

                    virtual QString getMetaData( const QString& dataKey );
                    virtual QString getDBVersion();
                    void setPlayerUUID( QUuid id );
                    virtual QUuid getPlayerUUID();
                    virtual bool allowCollectStatistic();
                    virtual void setAllowCollectStatistic( bool allowCollectStatistic );

                    virtual bool allowNetworkConnection();
                    virtual void setAllowNetworkConnection( bool alllowNetworkConnection );

                    virtual bool getShowUserSettingsDialog();
                    virtual void setShowUserSettingsDialog( bool showUserSettingsDialog );

                private:
                    SystemMetaDataRepositoryFake() {}

                    QHash<QString, QString> _metaData;
            };
        }
    }
}

#endif // SYSTEMMETADATAREPOSITORYFAKE_H
