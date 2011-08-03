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
#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace RedBullPlayer {
    namespace Tools {
        namespace Network {
            class NetworkAccessManager : public QObject {
                    Q_OBJECT


                public:
                    NetworkAccessManager( QObject* parent );
                    void get( QUrl url );
                    void abort();

                signals:
                    void finished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength );
                    void failed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase );
                    void progress( int percent );

                private:
                    QNetworkAccessManager* _manager;
                    int _lastPercent;
                    QNetworkReply* _reply;

                private slots:
                    void requestFinished( QNetworkReply* reply );
                    void downloadProgress( qint64 bytesReceived, qint64 bytesTotal );

            };
        }
    }
}

#endif // NETWORKACCESSMANAGER_H
