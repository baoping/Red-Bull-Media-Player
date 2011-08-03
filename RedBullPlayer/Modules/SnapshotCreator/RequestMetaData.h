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
#ifndef REQUESTMETADATA_H
#define REQUESTMETADATA_H

#include <QObject>
#include <QUuid>

namespace RedBullPlayer {
    namespace Modules {
        namespace SnapshotCreator {

            class RequestMetaData {
                public:
                    explicit RequestMetaData( QUuid id );
                    explicit RequestMetaData( QUuid id, QString mediaFileName, float position );
                    explicit RequestMetaData( QUuid id, QString mediaFileName, qint64 time );
                    explicit RequestMetaData( QUuid id, QString mediaFileName, float position, qint64 time );

                    QUuid id() const {
                        return _id;
                    }
                    QString mediaFileName() const {
                        return _mediaFileName;
                    }
                    float position() const {
                        return _position;
                    }
                    qint64 time() const {
                        return _time;
                    }

                    void setMediaFileName( QString mediaFileName ) {
                        _mediaFileName = mediaFileName;
                    }
                    void setPosition( float position ) {
                        _position = position;
                    }
                    void setTime( qint64 time ) {
                        _time = time;
                    }

                private: //data
                    QUuid _id;
                    QString _mediaFileName;
                    float _position;
                    qint64 _time;

            };
        }
    }
}


#endif // REQUESTMETADATA_H
