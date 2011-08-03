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
#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H


// Qt includes
#include <QObject>

// Project includes
#include "../Interfaces/IError.h"
#include "SnapshotCreationData.h"

namespace RedBullPlayer {
    namespace SnapshotCreator {

        class ArgumentParser : public QObject {
                Q_OBJECT
            public:
                explicit ArgumentParser( QObject *parent = 0 );

                SnapshotCreationData* parseCommand( QString command, RedBullPlayer::IError* error );
            signals:

            public slots:



        };
    }
}
#endif // ARGUMENTPARSER_H
