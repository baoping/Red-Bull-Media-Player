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
#ifndef CHAPTERCARROUSEL_H
#define CHAPTERCARROUSEL_H

// Qt Includes
#include <QObject>
#include <QLabel>

// Animation
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qteasingcurve.h"
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qtparallelanimationgroup.h"

// Project Includes
#include "CarrouselItem.h"
#include "../../Interfaces/IChapter.h"
#include "../../Events/Player/PlayerTimeChangedEvent.h"


class ChapterCarrousel : public QObject {
        Q_OBJECT

    public:
        ChapterCarrousel( QWidget *parent );
        void scrollLeft();
        void scrollRight();
        void setItems( QList<RedBullPlayer::Modules::ChapterSelect::IChapter*> items, int selectedChaperIndex );
        bool isBeginning();
        bool isEnd();
        void update( bool displayAccordingToCurrIndex = true );
        void changeItemChapterImage( int indexOfItem, QImage chapterImage, bool immediateUpdate );
        void setCurrentVideoLengthInSeconds( int currentVideoLengthInSeconds );
        int getItemCount();
        void selectChapterItem( int chapterIndex );

    signals:
        void itemClicked( int time );
        void itemsRepositioned();

    public slots:
        void itemClickedSlot( int id );
        void animationFinishedSlot();

    protected:
        void initItems();
        void displayItems( bool displayAccordingToCurrIndex = true );
        void clearItems();
        void displayItem( CarrouselItem* widget, int indexOnScreen );
        int getItemXPos( int indexOnScreen );
        int getItemYPos( int indexOnScreen );
        int getItemWidth();
        int getItemHeight();
        void sort( QList<RedBullPlayer::Modules::ChapterSelect::IChapter*> *_items );
        void addWidgetAnimation( QWidget *w, QtParallelAnimationGroup *group, QPoint startPos, QPoint endPos );
        bool event ( QEvent * e );
        void onPlayerTimeChanged( RedBullPlayer::Events::PlayerTimeChangedEvent* e );

    private:
        QWidget *_parentWidget;
        QList<RedBullPlayer::Modules::ChapterSelect::IChapter*> _items;
        QList<CarrouselItem*> _widgets;
        int _currentIndex;
        int _firstIndexOnScreen;
        int _currentVideoLengthSeconds;
        bool _userSelectedChapter;
        bool _animationOn;
};

#endif // CHAPTERCARROUSEL_H
