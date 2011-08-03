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
#ifndef CHAPTERIMAGEREQUESTEVENT_H
#define CHAPTERIMAGEREQUESTEVENT_H

#include <QEvent>
#include <QString>
#include <QList>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT ChapterImageRequestEvent : public QEvent {
            public:
                ChapterImageRequestEvent( QUuid mediaId, float position, uint second ) : QEvent( ChapterImageRequest ) {
                    _mediaId = mediaId;
                    _position = position;
                    _second = second;
                }

                QUuid mediaId() const {
                    return _mediaId;
                }
                float position() const {
                    return _position;
                }
                uint second() const {
                    return _second;
                }

            private:
                ChapterImageRequestEvent() : QEvent( ChapterImageRequest ) {}
                QUuid _mediaId;
                float _position;
                uint _second;
        };
    }
}

#endif // CHAPTERIMAGEREQUESTEVENT_H
