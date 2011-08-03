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
#ifndef PBUTTON_H
#define PBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QMap>
#include <QSize>

#include "Enumerations.h"

namespace RedBullPlayer {
    namespace Widgets {
        class CommandButton : public QPushButton {
                Q_OBJECT

            public:
                CommandButton( QWidget *parent,
                               QString normalImageUrl,
                               int w = 0,
                               int h = 0 );
                ~CommandButton();

                virtual QSize sizeHint() const;

                void setMouseOverImage( QString imageUrl );
                void setImage( QString imageUrl );
                void setDisabledImage( QString imageUrl );

                bool mouseOverEnabled() const {
                    return _mouseOverEnabled;
                }

                void enableMouseOver() {
                    _mouseOverEnabled = true;
                }

                void disableMouseOver() {
                    _mouseOverEnabled = false;
                }

                virtual void enterEvent( QEvent * );
                virtual void leaveEvent( QEvent * );
                virtual void paintEvent( QPaintEvent *event );

            public slots:
                virtual void setEnabled( bool enabled );

            private:
                QPixmap getImage( ButtonImageType imageType );
                void addImage( ButtonImageType imageType, QString imageUrl );

                int _width;
                int _height;

                bool _mouseOverEnabled;
                bool _isMouseOver;
                QSize _imageSize;

                QMap<ButtonImageType, QPixmap*>* _images;

        };
    }
}

#endif // PBUTTON_H
