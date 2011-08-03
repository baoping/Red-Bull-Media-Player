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
#ifndef RSSFEEDPARSERQUEUE_H
#define RSSFEEDPARSERQUEUE_H

#include <QObject>
#include <QUuid>
#include <QQueue>

#include "RssFeedParser.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace XmlNewsFeed {

            class RssFeedParserQueue : public QObject {
                    Q_OBJECT
                public:
                    RssFeedParserQueue( QObject *parent = 0 );
                    ~RssFeedParserQueue();

                    void enqueueParse( QUuid parseId, QString url );

                private: //data
                    RssFeedParser* _parser;
                    QUuid _currentJob;
                    QQueue< QPair<QUuid, QString> > _jobs;
                private: //functions
                    void doNextParse();
                signals:
                    void parseSuccessful( QUuid jobId, RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
                    void parseFailed( QUuid jobId );

                private slots:
                    void parserSuccessful( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
                    void parserFailed();

            };
        }
    }
}

#endif // RSSFEEDPARSERQUEUE_H
