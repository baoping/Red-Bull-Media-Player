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
#ifndef CENTRALUICREATOR_H
#define CENTRALUICREATOR_H

// QT includes
#include <QtGui/QStackedWidget>
#include <QPixmap>

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../Events/CentralUIWidgetChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"

namespace Ui {
    class CentralUICreator;
}

class CentralUICreator : public QStackedWidget {
        Q_OBJECT
    public:
        CentralUICreator( RedBullPlayer::PlayerShell::Shell* shell, QWidget *parent = 0 );
        ~CentralUICreator();

        void setCurrentWidget( RedBullPlayer::Events::CentralUIWidgetChangedEvent::CentralUIWidgets widget );
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void handlePlayerVisibilityChanged( bool visible );

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent *e );
        void enterEvent( QEvent *e );

    private:
        Ui::CentralUICreator *_ui;
        RedBullPlayer::PlayerShell::Shell *_shell;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _currentFocusedCentralWidgetType;
};

#endif // CENTRALUICREATOR_H
