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
#ifndef SNAPSHOTCREATIONREQUESTDATA_H
#define SNAPSHOTCREATIONREQUESTDATA_H

#include "../../Interfaces/ISnapshotCreationRequest.h"

#include <QString>
#include <QUuid>

// Project Includes
#include "../Events_global.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT SnapshotCreationRequestData : public RedBullPlayer::Modules::SnapshotCreator::ISnapshotCreationRequestData {
            public:
                SnapshotCreationRequestData( QUuid id, QString movieFileName, float position, uint second = 70 ) {
                    Q_ASSERT( ! id.isNull() );
                    Q_ASSERT( ! movieFileName.isNull() );
                    Q_ASSERT( position >= 0 );
                    Q_ASSERT( position <= 1 );
                    _id = id;
                    _movieFileName = movieFileName;
                    _position = position;
                    _second = second;
                }
                virtual QUuid id() const {
                    return _id;
                }
                virtual QString movieFileName() const {
                    return _movieFileName;
                }
                virtual float position() const {
                    return _position;
                }
                virtual uint second() const {
                    return _second;
                }

            private:
                QUuid _id;
                QString _movieFileName;
                float _position;
                uint _second;
        };
    }
}
#endif // SNAPSHOTCREATIONREQUESTDATA_H
