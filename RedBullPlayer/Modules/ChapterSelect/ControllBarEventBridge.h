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
#ifndef CHAPTER_SELECT_CONTROLLBAREVENTBRIDGE_H
#define CHAPTER_SELECT_CONTROLLBAREVENTBRIDGE_H

#include <QObject>

//Project includes
#include "../../Events/ControllBar/ControllBarPreviewButtonClickedEvent.h"
#include "../../Events/ControllBar/ControllBarVisibilityChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {

            class ControllBarEventBridge : public QObject {
                    Q_OBJECT
                public:
                    explicit ControllBarEventBridge( QObject *parent = 0 );
                    ~ControllBarEventBridge();

                protected:
                    bool event( QEvent* event );

                    void onControllBarPreviewButtonClicked( RedBullPlayer::Events::ControllBarPreviewButtonClickedEvent* e );
                    void onControllBarVisibilityChanged( RedBullPlayer::Events::ControllBarVisibilityChangedEvent* e );

                signals:
                    void previewClicked();
                    void controllBarVisibilityChanged( bool controllBarVisible );
            };
        }
    }
}


#endif // CONTROLLBAREVENTBRIDGE_H
