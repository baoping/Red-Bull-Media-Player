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
#ifndef ALBUMCOVERCOLLECTORMODULE_H
#define ALBUMCOVERCOLLECTORMODULE_H

//QT Includes
#include <QObject>
#include <QSettings>
#include <QList>
#include <QPair>

// Project Includes

#include "../../Interfaces/IAlbumCoverCollector.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/RequestAlbumCoverImageEvent.h"
#include "../../Events/MetaDataChangedEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"


#include "Collector.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace AlbumCoverCollector {
            class AlbumCoverCollectorModule : public IAlbumCoverCollector {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::AlbumCoverCollector::IAlbumCoverCollector )

                public:
                    AlbumCoverCollectorModule();
                    ~AlbumCoverCollectorModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "AlbumCoverCollector";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                    virtual void collectAlbumCover( QUuid mediaGuid, bool allowRequestMetaData );

                protected:
                    bool event( QEvent * e );
                    void onRequestAlbumCoverImage( RedBullPlayer::Events::RequestAlbumCoverImageEvent *event );
                    void onMetaDataChanged( RedBullPlayer::Events::MetaDataChangedEvent *event );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent* event );
                    void onUserSettingsChanged( RedBullPlayer::Events::UserSettingsChangedEvent *event );

                private:
                    void removeCollector( QUuid mediaGuid );
                    void bufferRequest( QUuid requestId, bool metaData );
                    void resolveMediaLibrary();
                    void resumeAlbumCoverCollection();

                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;

                    bool _allowCollect;
                    bool _internetAvailable;

                    QList<RedBullPlayer::Modules::AlbumCoverCollector::Collector*>* _collectors;

                    QList<QPair<QUuid, bool>*> _requestBuffer;

                private slots:
                    void slotImageFound( QUuid );
                    void slotNoImageFound( QUuid );
            };
        }
    }
}

#endif // ALBUMCOVERCOLLECTORMODULE_H
