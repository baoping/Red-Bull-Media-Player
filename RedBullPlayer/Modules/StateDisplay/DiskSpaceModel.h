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
#ifndef DISKSPACEMODEL_H
#define DISKSPACEMODEL_H

#include <QObject>

// Project Includes
#include "../../Interfaces/StateDisplay/IDiskSpaceModel.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class DiskSpaceModel : public IDiskSpaceModel {
                    Q_OBJECT
                public:
                    DiskSpaceModel( QObject *parent = 0 );
                    virtual ~DiskSpaceModel();

                    virtual QString freeSpaceString();
                    virtual QString maximumSpaceString();

                private:
                    QString convertBytesToHumanReadableString( quint64 bytes );

            };

        } // namespace StateDisplay
    } // namespace Modules
} // namespace RedBullPlayer

#endif // DISKSPACEMODEL_H
