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
#ifndef DISKSPACEBUTTON_H
#define DISKSPACEBUTTON_H

#include "StatusDisplayButton.h"

namespace RedBullPlayer {
    namespace Widgets {
        class DiskSpaceButton : public StatusDisplayButton {
                Q_OBJECT
            public:
                DiskSpaceButton( QString normalImageUrl,
                                 QString progressImageUrl,
                                 QWidget *parent,
                                 int borderLeft = 0,
                                 int borderRight = 0,
                                 int borderTop = 0,
                                 int borderBottom = 0
                               );

                void setPercent( int percent ) {
                    _percent = percent;
                }

                int percent() const {
                    return _percent;
                }

                virtual void paintEvent( QPaintEvent *event );

            private:
                QPixmap* _progressImage;
                int _percent;
                int _borderLeft;
                int _borderRight;
                int _borderTop;
                int _borderBottom;
        };
    }
}


#endif // DISKSPACEBUTTON_H
