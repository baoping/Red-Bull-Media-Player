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
#ifndef DOWNLOADUTIL_H
#define DOWNLOADUTIL_H

//Qt includes
#include <QObject>
#include <QtCore/QUrl>
#include <QDir>
#include <QtNetwork/QNetworkReply>

#include "NetworkAccessManager.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace Network {
            class DownloadUtil : public QObject {
                    Q_OBJECT

                public:
                    DownloadUtil( QObject* parent = 0 );
                    ~DownloadUtil();

                    QString targetDirectory() const {
                        return _targetDirectory;
                    }
                    void setTargetDirectory( QString directory );

                    QString loggerName() const {
                        return _loggerName;
                    }
                    void setLoggerName( QString loggerName ) {
                        _loggerName = loggerName;
                    }

                    QString fileName() const {
                        return _fileName;
                    }
                    QString contentType() const {
                        return _contentType;
                    }
                    QString contentLength() const {
                        return _contentLength;
                    }
                    QUrl requestUrl() const {
                        return _requestUrl;
                    }
                    int httpStatusCode() const {
                        return _httpStatusCode;
                    }
                    QString httpReasonPhrase() const {
                        return _httpReasonPhrase;
                    }

                    void downloadFile( QUrl url );

                    void cancelDownload();

                signals:
                    void downloadFinished( QString fileName, QString contentType, QString contentLength );
                    void downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase );
                    void progress( int percent );

                private slots:
                    void namFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength );
                    void namFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase );

                private:
                    void bootstrap();

                    QString _targetDirectory;
                    QString _loggerName;
                    QString _fileName;
                    QString _contentType;
                    QString _contentLength;
                    QUrl _requestUrl;
                    int _httpStatusCode;
                    QString _httpReasonPhrase;
                    RedBullPlayer::Tools::Network::NetworkAccessManager* _nam;


            };
        }
    }
}

#endif // DOWNLOADUTIL_H
