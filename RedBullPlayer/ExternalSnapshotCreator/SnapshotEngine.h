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
#ifndef SNAPSHOTENGINE_H
#define SNAPSHOTENGINE_H

#include <QObject>
#include <QUuid>

// #include "InputParser.h"

class IMediaPlayer;

namespace RedBullPlayer {
    namespace SnapshotCreator {

        class SnapshotEngine : public QObject {
                Q_OBJECT

            public:
                SnapshotEngine( QObject* parent );
                ~SnapshotEngine();
                void requestNewSnapshot( QUuid requestId, QString movieFileName, float position );
                void requestMetaData( QUuid requestId, QString movieFileName );
                bool readyForNewSnapshot() const {
                    return _readyForNewSnapshot;
                }

            protected:
                virtual bool event( QEvent* event );
                //	void writeToConsole( QString msg );
                void positionChanged();
                void playing();

            private: // DATA
                enum RequestType {
                    Snapshot = 0,
                    MetaData = 1
                };

                IMediaPlayer* _mpInterface;
                // QObject *_inputParser;
                QUuid _requestId;
                float _position;
                bool _readyForNewSnapshot;
                RequestType _requestType;
            signals:
                void snapshotCreationFaild( QString error );
                void snapshotCreated( QString requestId, QString fileName, float position );
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

        };
    }
}
#endif // SNAPSHOTENGINE_H
