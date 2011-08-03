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
#ifndef PODCASTCACHE_H
#define PODCASTCACHE_H

//Qt includes
#include <QObject>
#include <QUuid>
#include <QList>

//Project includes
#include "../../Interfaces/RssFeed/IRssFeed.h"
#include "../../Interfaces/MediaLibrary/IContent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

//Module includes
#include "ContentDescriptor.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class PodCastCache : public QObject {
                    Q_OBJECT
                public:
                    PodCastCache( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent = 0 );
                    ~PodCastCache();

                    bool cacheAvailable();
                    void setRssFeedImage( QString fileName );
                    void setRssFeedItemImage( QString guid, QImage image );
                    void setRssFeedItemLocalFileName( QString guid, QString localFileName );
                    void saveRssFeedItemToDb( QString guid );
                    void skipContent( QString contentId );
                    void resetSkipContent( QString contentId );
                    bool isSkippedContent( QString contentId );
                    bool isAvailable( QString contentId );
                    void banContent( QString contentId );

                    ContentDescriptor* getContentDescriptorById( QString contentId );

                public slots:
                    void handleRssFeed( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );

                private:

                    RedBullPlayer::Tools::RSS::IRssFeed* _rssFeed;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;

                    QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* _availableContent;
                    QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* _downloadedContent;
                    QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* _skippedContent;

                    void updateDownloadedContent();
                    void updateSkippedContent();

                    void saveToMediaDb( RedBullPlayer::Modules::MediaLibrary::IContent* content );

                signals:
                    void parseStarted();
                    void parseFinished();
            };
        }
    }
}

#endif // PODCASTCACHE_H
