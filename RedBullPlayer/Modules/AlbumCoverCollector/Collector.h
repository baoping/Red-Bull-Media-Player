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
#ifndef COLLECTOR_H
#define COLLECTOR_H

//QT Includes
#include <QObject>

//Project includes
#include "../../ToolLib/Network/DownloadUtil.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace RedBullPlayer {
    namespace Modules {
        namespace AlbumCoverCollector {
            class Collector : public QObject {
                    Q_OBJECT

                public:
                    Collector( QObject * parent = 0 );

                    void searchAlbumCover( QUuid mediaGuid, bool allowRequestMetaData );

                    QUuid getCurrentRequestGuid() const {
                        return _currentRequestMediaGuid;
                    }

                private:
                    void getAlbumCover( QUrl url );
                    void resolveMediaLibrary();
                    void parseAmazonResponse( QByteArray data );
                    void parseMusicBrainzResponse( QByteArray data );

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;

                    QNetworkAccessManager *_albumCollector;
                    QNetworkAccessManager *_musicBrainz;

                    RedBullPlayer::Tools::Network::DownloadUtil* _imageDownloader;

                    QUuid _currentRequestMediaGuid;

                private slots:
                    void slotCollectorFinished( QNetworkReply * );
                    void slotMusicBrainzFinished( QNetworkReply * );
                    void slotImageDownloadFinished( QString, QString, QString );
                    void slotImageDownloadFailed( QString, int, QString );

                signals:
                    void signalImageFound( QUuid mediaGuid );
                    void signalNoImageFound( QUuid mediaGuid );

            };
        }
    }
}

#endif // COLLECTOR_H
