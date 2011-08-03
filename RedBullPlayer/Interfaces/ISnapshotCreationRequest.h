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
#ifndef ISNAPSHOTCREATIONREQUEST_H
#define ISNAPSHOTCREATIONREQUEST_H

#include <QUuid>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace SnapshotCreator {
            class ISnapshotCreationRequestData {
                public:
                    virtual ~ISnapshotCreationRequestData() {}
                    virtual QUuid id() const = 0;
                    virtual QString movieFileName() const = 0;
                    virtual float position() const = 0;
                    virtual uint second() const = 0;
            };

            class ISnapshotCreationResponseData {
                public:
                    virtual ~ISnapshotCreationResponseData() {}
                    virtual QUuid requestId() const = 0;
                    virtual QString snapshotFileName() const = 0;
            };
        }
    }
}
#endif // ISNAPSHOTCREATIONREQUEST_H
