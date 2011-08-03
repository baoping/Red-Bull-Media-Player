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
#ifndef SEARCHUICREATOR_H
#define SEARCHUICREATOR_H

#include <QtGui/QWidget>

//Projects Includes
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Widgets/CommandButton.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"

namespace Ui {
    class SearchUICreator;
}

class SearchUICreator : public QWidget {
        Q_OBJECT
    public:
        SearchUICreator( QWidget *parent = 0 );
        ~SearchUICreator();

    public:
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void enableSearchGlobally();
        void resolveStyleFactory();
        void setupStyle();

    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );
        void emitMainUIRegionFocusedChangedEvent();
        void setupTitleStyleLight();
        void setupTitleStyleDark();
        void setupSearchBoxStyle();

    protected slots:
        void searchTextChanged( QString query );
        void clearButtonClicked();

    private:
        Ui::SearchUICreator *_ui;

        bool _searchEnabledForCurrentPlaylist;
        bool _searchGloballyEnabled;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        QPixmap _backgroundImage;
        QPixmap _backgroundImageSearchTextBox;
        QPixmap _imageSearchBoxHead;
        QPixmap _imageSearchBoxCover;
        QPixmap _imageGradientLine;
        float _grayLineWidthToTotalWidthRatio;

        RedBullPlayer::Widgets::CommandButton* _clearButton;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
        QString _titleStyleSheetLight;
        QString _titleStyleSheetDark;
};

#endif // SEARCHUICREATOR_H
