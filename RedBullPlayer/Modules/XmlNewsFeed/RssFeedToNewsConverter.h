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
#ifndef RSSFEEDTONEWSCONVERTER_H
#define RSSFEEDTONEWSCONVERTER_H

#include <QObject>
#include <QTextBrowser>
#include <QList>
#include <QQueue>
#include <QHash>

#include "../../Interfaces/MediaLibrary/INews.h"
#include "../../Interfaces/RssFeed/IRssFeedItem.h"
#include "../../ToolLib/Network/DownloadUtil.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace XmlNewsFeed {

            class RssFeedToNewsConverter : public QObject {
                    Q_OBJECT
                public:
                    RssFeedToNewsConverter( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent = 0 );
                    ~RssFeedToNewsConverter();

                    void convert( RedBullPlayer::Tools::RSS::IRssFeedItem* rssFeedItem );

                private: //data
                    RedBullPlayer::Tools::Network::DownloadUtil* _imageLoader;
                    QQueue<QString> _imagesToLoad;
                    QList<QString> _imagesLoaded;
                    RedBullPlayer::Tools::RSS::IRssFeedItem* _currentItem;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                private: //functions
                    void loadNextImage();
                    void convertToNews();

                private slots:
                    void downloadFinished( QString fileName, QString contentType, QString contentLength );
                    void downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase );

                signals:
                    void convertFinished( RedBullPlayer::Modules::MediaLibrary::INews* news, QList<RedBullPlayer::Modules::MediaLibrary::INewsImage*>* newsImages );

            };
        }
    }
}

#endif // RSSFEEDTONEWSCONVERTER_H
