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
#ifndef RSSFEEDPARSEREVENTBRIDGE_H
#define RSSFEEDPARSEREVENTBRIDGE_H

#include <QObject>

#include "RssFeedParserQueue.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"
#include "../../Events/RssFeed/RssFeedParseRequestEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace XmlNewsFeed {

            class RssFeedParserEventBridge : public QObject {
                    Q_OBJECT
                public:
                    RssFeedParserEventBridge( RssFeedParserQueue* queue, QObject *parent = 0 );
                    ~RssFeedParserEventBridge();

                protected:
                    bool event( QEvent* e );

                    void onRssFeedParseRequestEvent( RedBullPlayer::Events::RssFeedParseRequestEvent* ev );

                private: //data
                    RssFeedParserQueue* _queue;

                private slots:
                    void parseSuccessful( QUuid jobId, RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
                    void parseFailed( QUuid jobId );

            };
        }
    }
}

#endif // RSSFEEDPARSEREVENTBRIDGE_H
