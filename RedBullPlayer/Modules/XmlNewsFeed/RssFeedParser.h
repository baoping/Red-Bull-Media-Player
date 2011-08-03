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
#ifndef RSSFEEDPARSER_H
#define RSSFEEDPARSER_H

#include <QObject>

#include "../../ToolLib/Network/NetworkAccessManager.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace XmlNewsFeed {

            class RssFeedParser : public QObject {
                    Q_OBJECT
                public:
                    RssFeedParser( QObject *parent = 0 );
                    ~RssFeedParser();

                    void parseRssFeed( QString rssFeedUrl );

                    bool isRunning() const {
                        return _isRunning;
                    }

                private:
                    RedBullPlayer::Tools::Network::NetworkAccessManager* _nam;
                    bool _isRunning;

                signals:
                    void parseSuccessful( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
                    void parseFailed();

                private slots:
                    void getFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength );
                    void getFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase );

            };

        } // namespace XmlNewsFeed
    } // namespace Modules
} // namespace RedBullPlayer

#endif // RSSFEEDPARSER_H
