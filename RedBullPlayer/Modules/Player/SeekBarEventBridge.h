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
#ifndef PLAYER_SEEKBAREVENTBRIDGE_H
#define PLAYER_SEEKBAREVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/SeekBar/SeekBarPlayHeadChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {

            class SeekBarEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit SeekBarEventBridge( QObject *parent = 0 );
                    ~SeekBarEventBridge();

                    void updateSeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                protected:
                    bool event( QEvent* e );

                    void onSeekBarPlayHeadChanged( RedBullPlayer::Events::SeekBarPlayHeadChangedEvent* e );

                signals:
                    void seekBarPlayerHeadChanged( float position );
            };
        }
    }
}

#endif // SEEKBAREVENTBRIDGE_H
