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
#ifndef BULKNEWCONTENTAVAILABLEEVENT_H
#define BULKNEWCONTENTAVAILABLEEVENT_H

#include <QEvent>
#include <QList>

#include "../Events_global.h"
#include "../EventTypes.h"
#include "../../Interfaces/Enumerations/ContentCategoryEnum.h"
#include "../../Interfaces/MediaLibrary/IContent.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT BulkNewContentAvailableEvent : public QEvent {
            public:
                BulkNewContentAvailableEvent( QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* content,
                                              RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum category ) : QEvent( BulkNewContentAvailable ) {
                    _content = content;
                    _category = category;
                }

                QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* content() const {
                    return _content;
                }
                RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum category() const {
                    return _category;
                }

            private:
                BulkNewContentAvailableEvent() : QEvent( BulkNewContentAvailable ) {}

                QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* _content;
                RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum _category;
        };
    }
}


#endif // BULKNEWCONTENTAVAILABLEEVENT_H
