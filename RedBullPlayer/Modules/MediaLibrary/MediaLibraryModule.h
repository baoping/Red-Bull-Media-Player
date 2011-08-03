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
#ifndef MEDIALIBRARYMODULE_H
#define MEDIALIBRARYMODULE_H

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

// Qt includes
#include <QObject>
#include <QString>

// Project includes
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/IMediaMetadataHandler.h"
#include "../../Interfaces/MediaLibrary/ISearchEngine.h"
#include "../../Interfaces/ISnapshotCreationRequest.h"
#include "../../Events/SnapshotCreator/SnapshotCreatedEvent.h"
#include "MediaDB.h"
#include "MediaMetadataHandler.h"
#include "SearchEngine.h"
namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class MediaLibraryModule : public IMediaLibrary {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary )

                public:
                    MediaLibraryModule();

                    virtual void Setup( QObject* shell, QDomElement config );

                    virtual QString name() const {
                        return "MediaLibrary";
                    }

                    virtual QString version() const {
                        return "1.0.0";
                    }

                    virtual IMediaDB* mediaDB() const {
                        return _mediaDB;
                    }

                    virtual IMediaMetadataHandler* metadataHandler() const {
                        return _metadataHandler;
                    }

                    virtual ISearchEngine* searchEngine() const {
                        return _searchEngine;
                    }
                private:
                    MediaDB* _mediaDB;
                    IMediaMetadataHandler* _metadataHandler;
                    ISearchEngine* _searchEngine;
                    RedBullPlayer::PlayerShell::Shell* _shell;

            };
        }
    }
}





#endif // MEDIALIBRARYMODULE_H
