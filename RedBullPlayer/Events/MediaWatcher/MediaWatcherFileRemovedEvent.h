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
#ifndef MEDIAWATCHERFILEREMOVEDEVENT_H
#define MEDIAWATCHERFILEREMOVEDEVENT_H

//QT Includes
#include <QEvent>
#include <QUuid>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "../../ToolLib/UuidCreator.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT MediaWatcherFileRemovedEvent : public QEvent {
            public:
                MediaWatcherFileRemovedEvent( QString fileName, QUuid requestId = RedBullPlayer::Tools::UuidCreator::create() ) : QEvent( MediaWatcherFileRemoved ) {
                    _fileName = fileName;
                    _requestId = requestId;
                }

                QUuid requestId() const {
                    return _requestId;
                }
                QString fileName() const {
                    return _fileName;
                }

            private:
                QUuid _requestId;
                QString _fileName;
        };
    }
}

#endif // MEDIAWATCHERFILEREMOVEDEVENT_H
