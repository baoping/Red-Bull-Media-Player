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
#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <QObject>
#include <QString>
#include <QStack>
#include <QTimer>
#include <QUuid>

#include "SnapshotCreationData.h"
#include "SnapshotEngine.h"

namespace RedBullPlayer {
    namespace SnapshotCreator {

        class InputParser : public QObject {
                Q_OBJECT

            public:
                InputParser( QObject* parent );
                void run();
                void writeToConsole( QString msg );
            public slots:
                void readNextCommand();

            private slots:
                void snapShotCreated( QString requestId, QString fileName, float position  );
                void snapShotCreationFailed( QString error );
                void metaDataReceived( QString requestId,
                                       QString title,
                                       QString artist,
                                       QString genre,
                                       QString copyright,
                                       QString album,
                                       QString trackNumber,
                                       QString description,
                                       QString rating,
                                       QString date,
                                       QString setting,
                                       QString url,
                                       QString language,
                                       QString nowPlaying,
                                       QString publisher,
                                       QString encodedBy,
                                       QString artworkUrl,
                                       QString trackID );
            private:
                enum WorkState {
                    InvalidCommand = 0,
                    StartedWork = 1,
                    StillBusy = 2
                };


                void sendReadyForInput();
                void sendBusy();
                void sendError( QString msg );

                InputParser::WorkState parseLine( QString line );
                SnapshotEngine* _se;
        };
    }
}

#endif // INPUTPARSER_H
