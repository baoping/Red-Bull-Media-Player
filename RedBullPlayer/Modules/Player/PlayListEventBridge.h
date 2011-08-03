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
#ifndef PLAYLISTEVENTBRIDGE_H
#define PLAYLISTEVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class PlayListEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit PlayListEventBridge( QObject *parent = 0 );
                    ~PlayListEventBridge();

                    void selectNextItem();
                    void selectPreviousItem();
                    void mediaItemContentChanged( QUuid mediaId );

                protected:
                    bool event( QEvent* event );
                    void onPlaylistItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );

                signals:
                    void playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* media,
                                               bool skipDocuments,
                                               bool fromSelectPreviousItemEvent,
                                               bool overrideFunctionSelectEvent );


            };
        }
    }
}

#endif // PLAYLISTEVENTBRIDGE_H
