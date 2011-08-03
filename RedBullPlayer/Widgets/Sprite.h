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
#ifndef SPRITE_H
#define SPRITE_H

#include "ClickableLabel.h"

namespace RedBullPlayer {
    namespace Widgets {
        class Sprite : public RedBullPlayer::Widgets::ClickableLabel {
                Q_OBJECT

            public:
                Sprite( QWidget *parent, int id = -1 );
                void setBgColor( QColor newColor );

            signals:
                void mouseOver();
                void mouseOver( int id );
                void mouseOut();
                void mouseOut( int id );

            protected:
                virtual void leaveEvent( QEvent *event );
                virtual void enterEvent( QEvent *event );
                virtual void paintEvent( QPaintEvent *event );

            private:
                QColor _color;
        };
    }
}

#endif // SPRITE_H
