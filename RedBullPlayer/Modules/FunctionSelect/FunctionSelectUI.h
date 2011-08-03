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
#ifndef FUNCTIONSELECTUI_H
#define FUNCTIONSELECTUI_H

//QT Includes
#include <QtGui/QWidget>

// Project Includes
#include "IconCarrousel.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"

namespace Ui {
    class FunctionSelectUI;
}

class FunctionSelectUI : public QWidget {
        Q_OBJECT
    public:
        FunctionSelectUI( QWidget *parent = 0 );
        ~FunctionSelectUI();
        QWidget *getMainWidget();
        void addWidget( QString url, QString activeUrl, QString darkUrl, QString darkActiveUrl, QString title );
        bool hasMainFocus();
        void selectItemInTheMiddle();
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
        void changeInternetConnectionStatus( bool isConnected );

    public slots:
        void onLeftNavigationClicked();
        void onRightNavigationClicked();
        void animationFinishedSlot();
        void iconClickedSlot( QString title );
        void iconDoubleClickedSlot( QString title );

    protected:
        void changeEvent( QEvent *e );
        void setNavigation();
        void paintEvent( QPaintEvent *e );

    private:
        Ui::FunctionSelectUI *_ui;
        IconCarrousel *_iconCarrousel;
        QImage _ellipse;

        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        QString _currentTitle;

    signals:
        void functionChanged( QString newFunctionTitle );
};

#endif // FUNCTIONSELECTUI_H
