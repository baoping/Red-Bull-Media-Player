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
#ifndef CHAPTERSELECTUI_H
#define CHAPTERSELECTUI_H

// Qt Includes
#include <QtGui/QWidget>
#include <QUuid>
#include <QHash>
#include <QQueue>

// Project Includes
#include "ChapterCarrousel.h"
#include "../../Interfaces/IChapter.h"
#include "../../Widgets/TransparentImage.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class ChapterSelectUI;
}

class ChapterSelectUI : public QWidget {
        Q_OBJECT

    public:
        ChapterSelectUI( QWidget *parent = 0 );
        ~ChapterSelectUI();
        void setItems( QList<RedBullPlayer::Modules::ChapterSelect::IChapter*> items, int selectedChapterIndex );
        void onVisibilityChange( bool visible );

        void setCurrentVideoLengthInSeconds( int seconds );
        void changeItemChapterImage( int indexOfItem, QImage chapterImage, bool immediateUpdate );


    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );

    private slots:
        void onLeftNavigationClicked();
        void onRightNavigationClicked();

        void carrouselItemClickedSlot( int time );
        void itemsRepositionedSlot();

    private:
        void initNavigationButtons();
        void updateNavigationButtons();

        RedBullPlayer::Widgets::CommandButton* createLeftNavigationButton();
        RedBullPlayer::Widgets::CommandButton* createRightNavigationButton();

        Ui::ChapterSelectUI *_ui;
        ChapterCarrousel *_carrousel;
        RedBullPlayer::Widgets::CommandButton *_left;
        RedBullPlayer::Widgets::CommandButton *_right;
        QPixmap _background;

    signals:
        void carrouselItemClicked( int time );

};

#endif // CHAPTERSELECTUI_H
