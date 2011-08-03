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
#ifndef BUTTON_H
#define BUTTON_H

#include "ClickableLabel.h"

namespace RedBullPlayer {
    namespace Widgets {
        class Button : public ClickableLabel {
            public:
                enum State {
                    NORMAL,
                    ONMOUSEDOWN,
                    ONMOUSEENTER,
                    ONMOUSELEAVE,
                    PRESSED
                };

            public:
                Button( QWidget *parent,
                        QString normalImgUrl,
                        QString onMouseDownImgUrl,
                        QString pressedImgUrl,
                        QString disabledImgUrl = "",
                        bool enableMouseOver = true,
                        QString mouseOverImgUrl = "" );
                ~Button();
                virtual QSize sizeHint() const;
                void setIcons( QString normalImgUrl,
                               QString onMouseDownImgUrl,
                               QString pressedImgUrl,
                               QString disabledImgUrl,
                               QString mouseOverImgUrl = "" );
                void setState( State newState );
                void setClickDownDistance( int distance );
                State state();
                virtual void setEnabled( bool enabled );

            protected:
                virtual void mousePressEvent ( QMouseEvent * event ) ;
                virtual void mouseReleaseEvent ( QMouseEvent * event ) ;
                virtual void enterEvent( QEvent * );
                virtual void leaveEvent( QEvent * );
                virtual void paintEvent( QPaintEvent *event );
                void deleteIcons();

            private:
                QPixmap *_normalIcon;
                QPixmap *_onMouseDownIcon;
                QPixmap *_pressedIcon;
                QPixmap *_disabledIcon;
                QPixmap *_currentIcon;
                QPixmap *_mouseOverIcon;
                State _currentState;
                bool _checked;
                int _clickDownDistance;
                bool _enableMouseOver;

        };
    }
}

#endif // BUTTON_H
