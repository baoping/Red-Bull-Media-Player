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
#ifndef MEDIAWATCHERMODULE_H
#define MEDIAWATCHERMODULE_H

#include <QObject>

#include "../../Interfaces/MediaWatcher/IMediaWatcher.h"
#include "../../PlayerShell/Shell.h"

#include "MediaWatcher.h"
#include "MediaWatcherEventBridge.h"
#include "MediaWatcherExclusionListHandler.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaWatcher {
            class MediaWatcherModule : public IMediaWatcher {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::MediaWatcher::IMediaWatcher )

                public:
                    MediaWatcherModule();
                    virtual ~MediaWatcherModule();

                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "MediaWatcher";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    MediaWatcher* _mediaWatcher;
                    MediaWatcherEventBridge* _mediaWatcherEventBridge;
                    MediaWatcherExclusionListHandler* _exclusionListHandler;
            };
        }
    }
}

#endif // MEDIAWATCHERMODULE_H
