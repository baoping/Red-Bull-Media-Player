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
#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QString>
#include <QPixmap>

namespace RedBullPlayer {
    namespace Widgets {
        class HelpWidget : public QWidget {
            public:
                HelpWidget( QWidget *parent, QString normalImageUrl, QString hoverImageUrl, QString helpText );

            protected:
                void enterEvent( QEvent *e );
                void leaveEvent( QEvent *e );
                void paintEvent( QPaintEvent * );

                void sendToolTipRequest();

            private:
                QPixmap _currentImage;
                QPixmap _image;
                QPixmap _hoverImage;
                QString _helpText;
                QSize _imageSize;
        };
    }
}

#endif // HELPWIDGET_H
