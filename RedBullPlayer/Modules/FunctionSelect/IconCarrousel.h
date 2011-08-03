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
#ifndef ICONCARROUSEL_H
#define ICONCARROUSEL_H

// QT includes
#include <QObject>

// Animation
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qteasingcurve.h"
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qtparallelanimationgroup.h"

// Project Includes
#include "../../Widgets/MirrorWidget.h"

class IconCarrousel : public QObject {
        Q_OBJECT

    public:
        IconCarrousel( QWidget *parentWidget );

        void scrollLeft();
        void scrollRight();
        int getCurrentIndex();
        RedBullPlayer::Widgets::MirrorWidget *getCurrentWidget();
        void addWidget( QString url, QString activeUrl, QString darkUrl, QString darkActiveUrl, QString title );
        void changeFocusStatus( bool hasFocus );
        void changeInternetConnectionStatus( bool isConnected );
        void selectIconInTheMiddle();


    public slots:
        void animationFinishedSlot();
        void iconClickedSlot( int id );

    protected:
        void initItems();
        void displayItems();
        void addWidgetAnimation( QWidget *w, QtParallelAnimationGroup *group, qreal startScaleFactor, QPoint startPos,
                                 qreal endScaleFactor, QPoint endPos );
        void appendWidget( QString url, QString activeUrl, QString darkImgUrl, QString darkActiveImgUrl, QString title, bool hideOnStart, int id );
        QPoint getActiveItemPos( RedBullPlayer::Widgets::MirrorWidget *activeWidget );
        QPoint getNonActiveItemPos( RedBullPlayer::Widgets::MirrorWidget *widget, bool left );
        int getPreviousIndex();
        int getNextIndex();
        int getNextVisibleItemIndexLeft();
        int getNextVisibleItemIndexRight();

    private:
        QWidget *_parentWidget;
        QList<RedBullPlayer::Widgets::MirrorWidget*> _items;
        int _currentIndex;   // index of icon in the middle (the biggest icon). This icon is not necessary the active one
        QSize ACTIVE_ITEM_SIZE;
        int ACTIVE_ITEM_Y_POS;
        int NON_ACTIVE_ITEM_Y_POS;
        int NON_ACTIVE_ITEM_DISTANCE;
        QPoint NON_ACTIVE_ITEM_POS1;
        QPoint NON_ACTIVE_ITEM_POS2;
        QtEasingCurve EASING_CURVE;
        bool _animationOn;

    signals:
        void animationFinished();
        void iconClicked( QString title );
        void iconDoubleClicked( QString title );

};
#endif // ICONCARROUSEL_H
