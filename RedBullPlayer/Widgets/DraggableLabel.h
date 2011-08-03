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
#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H

#include <QLabel>

namespace RedBullPlayer {
    namespace Widgets {
        class DraggableLabel : public QLabel {
                Q_OBJECT

            public:
                enum MouseEventType {
                    PRESS,
                    MOVE,
                    RELEASE
                };

                DraggableLabel( QWidget *parent );
                void invokeMouseEvent( MouseEventType type, QMouseEvent *ev );

            protected:
                void mousePressEvent ( QMouseEvent * ev ) ;
                void mouseMoveEvent ( QMouseEvent * ev ) ;
                void mouseReleaseEvent ( QMouseEvent * ev ) ;
                void mouseDoubleClickEvent ( QMouseEvent * ev ) ;

            signals:
                void dragging( QPoint draggingDistance );
                void doubleClicked();
                void clicked();

            private:
                QPoint _oldPoint;
                bool _draggingOn;
        };
    }
}

#endif // DRAGGABLELABEL_H
