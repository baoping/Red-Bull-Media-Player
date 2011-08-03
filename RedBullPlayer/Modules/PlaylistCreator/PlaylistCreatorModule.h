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
#ifndef PLAYLISTCREATORMODULE_H
#define PLAYLISTCREATORMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/IPlaylistCreator.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

#include "TVPlaylistCreator.h"
#include "NewsPlaylistCreator.h"
#include "AboutPlaylistCreator.h"
#include "DynamicDirectoryBasedPlaylistCreator.h"
#include "DynamicDirectoryBasedPlaylistCreatorThread.h"
#include "SearchDrivenPlaylistCreator.h"
#include "PlaylistItemEventHandler.h"
#include "SettingsPlaylistCreator.h"
#include "SettingsEventHandler.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class PlaylistCreatorModule : public IPlaylistCreator {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::PlaylistCreator::IPlaylistCreator )

                public:
                    PlaylistCreatorModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "PlaylistCreator";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                    virtual void SetupModuleConnections();

                private: // Methods
                    void resolveMediaLibrary();
                    void emitPlaylistClear();

                private: // DATA
                    DynamicDirectoryBasedPlaylistCreatorThread * _dynDirPlaylistCreatorThread;
                    TVPlaylistCreator* _tvPlaylistCreator;
                    NewsPlaylistCreator* _newsPlaylistCreator;
                    AboutPlaylistCreator *_aboutPlaylistCreator;
                    SearchDrivenPlaylistCreator* _searchDrivenPlaylistCreator;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    PlaylistItemEventHandler* _playlistItemEventHandler;
                    SettingsPlaylistCreator* _settingsPlaylistCreator;
                    SettingsEventHandler *_settingsEventHandler;

            };
        }
    }
}


#endif // PLAYLISTCREATORMODULE_H
