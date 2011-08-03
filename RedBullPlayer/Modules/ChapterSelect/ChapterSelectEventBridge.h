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
#ifndef CHAPTERSELECTEVENTBRIDGE_H
#define CHAPTERSELECTEVENTBRIDGE_H

#include <QObject>
#include <QUuid>

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/ChapterSelect/UpdateChapterSelectModelEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../Events/MediaLibrary/ChapterImageCreatedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {

            class ChapterSelectEventBridge : public QObject {
                    Q_OBJECT

                public:
                    explicit ChapterSelectEventBridge( QObject *parent = 0 );
                    ~ChapterSelectEventBridge();

                    void cancelChapterImageRequest( QUuid mediaId, float position );
                    void requestChapterImage( QUuid mediaId, float position, uint seconds );
                    void chapterSelected( int time );

                protected:
                    bool event( QEvent* event );

                    //ControllBar
                    void onUpdateChapterSelectModel( RedBullPlayer::Events::UpdateChapterSelectModelEvent* e );
                    //MediaPlayer
                    void onPlayerTimeChanged( RedBullPlayer::Events::PlayerTimeChangedEvent* e );
                    void onPlayerStateChanged( RedBullPlayer::Events::PlayerStateChangedEvent* e );
                    //PlayList
                    void onPlayListItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );
                    //UIResize
                    void onCentralUIRegionResized( RedBullPlayer::Events::CentralUIRegionResizedEvent* e );
                    //ChapterImage
                    void onChapterImageCreated( RedBullPlayer::Events::ChapterImageCreatedEvent* e );

                signals:
                    void updateChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    void centralUIRegionResized();
                    void chapterImageCreated( QUuid mediaId, float position );
            };
        }
    }
}

#endif // CHAPTERSELECTEVENTBRIDGE_H
