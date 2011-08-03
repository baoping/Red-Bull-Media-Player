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
#ifndef PLAYERSTARTEDURLGENERATOR_H
#define PLAYERSTARTEDURLGENERATOR_H

#include <QUuid>

#include "../../Interfaces/MediaLibrary/IStatisticEntry.h"
#include "../../Interfaces/Statistics/IUrlGenerator.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Statistics {
            class PlayerStartedUrlGenerator : public RedBullPlayer::Modules::Statistics::IUrlGenerator {
                public:
                    PlayerStartedUrlGenerator() : IUrlGenerator() {
                        _playerUuid = QUuid();
                        _apiKey = "";
                    };
                    virtual QUrl createRequestUrl( const RedBullPlayer::Modules::MediaLibrary::IStatisticEntry& entry );
                    virtual QUuid playerUuid() const {
                        return _playerUuid;
                    }

                    virtual void setPlayerUuid( QUuid uuid );
                    virtual void setApiKey( QString apiKey );
                private:
                    QUuid _playerUuid;
                    QString _apiKey;
            };
        }
    }
}
#endif // PLAYERSTARTEDURLGENERATOR_H
