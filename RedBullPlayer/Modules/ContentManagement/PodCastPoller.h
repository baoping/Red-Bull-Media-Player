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
#ifndef PODCASTPOLLER_H
#define PODCASTPOLLER_H

#include <QObject>
#include <QUuid>
#include <QTimer>

#include "../../Interfaces/RssFeed/IRssFeed.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class PodCastPoller : public QObject {
                    Q_OBJECT

                public:
                    PodCastPoller( QObject *parent = 0 );
                    ~PodCastPoller();
                    void startPoll( int interval );
                    void stopPoll();
                    bool canStart();
                    bool isPolling();

                protected:
                    bool event( QEvent* event );

                private:
                    QUuid _requestId;
                    QTimer* _pollTimer;

                private slots:
                    void pollRssFeed();

                signals:
                    void rssFeedRetrieved( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
            };

        }
    }
}

#endif // PODCASTPOLLER_H
