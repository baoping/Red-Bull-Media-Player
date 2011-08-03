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
#ifndef PLAYHEAD_H
#define PLAYHEAD_H

#include <QWidget>
#include <QMouseEvent>

class PlayHead : public QWidget {
        Q_OBJECT

    public:
        PlayHead( QWidget *parent );
        QSize sizeHint() const;

    signals:
        void draggingStart();
        void draggingEnd();
        void dragging( QPoint finalPoint );


    protected:
        void paintEvent( QPaintEvent * e );
        void mousePressEvent ( QMouseEvent * ev ) ;
        void mouseMoveEvent ( QMouseEvent * ev ) ;
        void mouseReleaseEvent ( QMouseEvent * ev ) ;

    private:
        QImage _img;
        QPoint _oldPoint;
        bool _draggingOn;

};

#endif // PLAYHEAD_H
