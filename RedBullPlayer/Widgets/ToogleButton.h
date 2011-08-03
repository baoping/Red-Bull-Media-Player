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
#ifndef TOOGLEBUTTON_H
#define TOOGLEBUTTON_H

#include <QToolButton>
#include <QPixmap>
#include <QMap>

#include "Enumerations.h"
#include "ImageListHandler.h"

namespace RedBullPlayer {
    namespace Widgets {
        class ToogleButton : public QToolButton, ImageListHandler {
            public:
                ToogleButton( QWidget *parent,
                              QString normalImageUrl,
                              QString checkedImageUrl );
                ~ToogleButton();

                virtual QSize sizeHint() const;

                void setMouseOverImage( QString imageUrl );
                void setMouseOverImageForToggledHover( QString imageUrl );
                void setImage( QString imageUrl );
                void setDisabledImage( QString imageUrl );
                void setCheckedImage( QString imageUrl );

                bool mouseOverEnabled() const {
                    return _mouseOverEnabled;
                }

                void enableMouseOver() {
                    _mouseOverEnabled = true;
                }

                void disableMouseOver() {
                    _mouseOverEnabled = false;
                }

            public slots:
                virtual void setEnabled( bool enabled );

            private:
                bool _mouseOverEnabled;
                bool _isMouseOver;

                virtual void enterEvent( QEvent * );
                virtual void leaveEvent( QEvent * );
                virtual void paintEvent( QPaintEvent *event );
        };
    }
}

#endif // TOOGLEBUTTON_H
