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
#ifndef PLAYER_CHAPTERSELECTEVENTBRIDGE_H
#define PLAYER_CHAPTERSELECTEVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/ChapterSelect/ChapterSelectedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {

            class ChapterSelectEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit ChapterSelectEventBridge( QObject *parent = 0 );
                    ~ChapterSelectEventBridge();

                    void updateChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                protected:
                    bool event( QEvent* e );
                    void onChapterSelected( RedBullPlayer::Events::ChapterSelectedEvent* e );

                signals:
                    void chapterSelected( int startTime );

            };
        }
    }
}

#endif // CHAPTERSELECTEVENTBRIDGE_H
