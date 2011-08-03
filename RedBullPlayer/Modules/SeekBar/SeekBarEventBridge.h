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
#ifndef SEEKBAREVENTBRIDGE_H
#define SEEKBAREVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/SeekBar/UpdateSeekBarModelEvent.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/Player/PlayerTrackStartedEvent.h"
#include "../../Events/Player/BufferingAnimationChangeVisibilityEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SeekBar {

            class SeekBarEventBridge : public QObject {
                    Q_OBJECT

                public:
                    explicit SeekBarEventBridge( QObject *parent = 0 );
                    ~SeekBarEventBridge();

                    void seekBarValueChanged( float value );

                protected:
                    bool event( QEvent* event );

                    //ControllBar
                    void onUpdateSeekBarModel( RedBullPlayer::Events::UpdateSeekBarModelEvent* e );
                    //MediaPlayer
                    void onPlayerTimeChanged( RedBullPlayer::Events::PlayerTimeChangedEvent* e );
                    void onPlayerStateChanged( RedBullPlayer::Events::PlayerStateChangedEvent* e );
                    void onPlayerTrackStarted( RedBullPlayer::Events::PlayerTrackStartedEvent* e );
                    //PlayList
                    void onPlayListItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );
                    //TV-Stream
                    void onBufferingAnimationChangeVisibility( RedBullPlayer::Events::BufferingAnimationChangeVisibilityEvent* e );

                signals:
                    void updateSeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    void playerTrackStarted();
                    void bufferingAnimationChangedVisibility( bool visible );

                public slots:

            };
        }
    }
}

#endif // SEEKBAREVENTBRIDGE_H
