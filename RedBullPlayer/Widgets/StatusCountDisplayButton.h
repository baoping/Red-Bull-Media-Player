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
#ifndef STATUSCOUNTDISPLAYBUTTON_H
#define STATUSCOUNTDISPLAYBUTTON_H

#include <QFont>
#include <QPainter>

#include "StatusDisplayButton.h"


namespace RedBullPlayer {
    namespace Widgets {
        class StatusCountDisplayButton : public RedBullPlayer::Widgets::StatusDisplayButton {
                Q_OBJECT
            public:
                StatusCountDisplayButton( int commandButtonWidth, int commandButtonHeight,
                                          QFont font, QColor borderColorCircle, qreal borderThickness,
                                          QColor backgroundColorCircle, QColor textColor,
                                          QString normalImageUrl, QString disabledImageUrl,
                                          QWidget *parent );

                void setNewCount( int count );

            protected:
                virtual void paintEvent( QPaintEvent *e );

                void drawCountGraphic( QPainter *painter );

            private:
                QPixmap _normalImage;
                QPixmap _disabledImage;
                QString _count;
                QFont _font;
                QColor _borderColorCircle;
                qreal _borderThickness;
                QColor _backgroundColorCircle;
                QColor _textColor;
        };
    }
}

#endif // STATUSCOUNTDISPLAYBUTTON_H
