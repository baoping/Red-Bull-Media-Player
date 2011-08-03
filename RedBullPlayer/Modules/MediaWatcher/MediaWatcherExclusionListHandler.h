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
#ifndef MEDIAWATCHEREXCLUSIONLISTHANDLER_H
#define MEDIAWATCHEREXCLUSIONLISTHANDLER_H

#include <QObject>
#include <QString>
#include <QList>

#include "../../Events/MediaWatcher/MediaWatcherEvents.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaWatcher {
            class MediaWatcherExclusionListHandler : public QObject {
                    Q_OBJECT

                public:
                    MediaWatcherExclusionListHandler( QObject *parent = 0 );
                    bool isExcluded( QString fileName );
                    void addFile( QString fileName );
                    void removeFile( QString fileName );

                protected:
                    bool event( QEvent* event );
                    void onMediaWatcherAddExclusion( RedBullPlayer::Events::MediaWatcherAddExclusionEvent* e );
                    void onMediaWatcherRemoveExclusion( RedBullPlayer::Events::MediaWatcherRemoveExclusionEvent* e );

                private:
                    QList<QString> _exclusionList;

            };
        }
    }
}

#endif // MEDIAWATCHEREXCLUSIONLISTHANDLER_H
