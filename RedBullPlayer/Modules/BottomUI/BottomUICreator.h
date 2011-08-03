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
#ifndef BOTTOMUICREATOR_H
#define BOTTOMUICREATOR_H

// QT includes
#include <QtGui/QWidget>
#include <QPixmap>

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"

namespace Ui {
    class BottomUICreator;
}

class BottomUICreator : public QWidget {
        Q_OBJECT
    public:
        BottomUICreator( RedBullPlayer::PlayerShell::Shell* shell, QWidget *parent = 0 );
        ~BottomUICreator();
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );

    public slots:
        void draggingSlot( QPoint distance );

    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );
        void initResizeWidget();
        void emitMainUIRegionFocusedChangedEvent();

    private:
        Ui::BottomUICreator *_ui;
        RedBullPlayer::PlayerShell::Shell *_shell;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;

        QPixmap _upperGradient;
        QPixmap _backgroundPattern;
        float _grayLineWidthToTotalWidthRatio;

    signals:
        void dragging( QPoint distance );
        void resizeWidgetClicked();
};

#endif // BOTTOMUICREATOR_H
