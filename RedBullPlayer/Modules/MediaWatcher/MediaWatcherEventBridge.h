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
#ifndef MEDIAWATCHEREVENTBRIDGE_H
#define MEDIAWATCHEREVENTBRIDGE_H

#include <QObject>

#include "MediaWatcher.h"
#include "MediaWatcherExclusionListHandler.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaWatcher {
            class MediaWatcherEventBridge : public QObject {
                    Q_OBJECT
                public:
                    MediaWatcherEventBridge( MediaWatcher* mediaWatcher, MediaWatcherExclusionListHandler* exclusionListHandler, QObject *parent = 0 );

                private:
                    MediaWatcher* _mediaWatcher;
                    MediaWatcherExclusionListHandler* _exclusionListHandler;

                private slots:
                    void fileAdded( QString fileName );
                    void fileRemoved( QString fileName );
            };
        }
    }
}

#endif // MEDIAWATCHEREVENTBRIDGE_H
