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
#ifndef TOPUICREATOR_H
#define TOPUICREATOR_H

// Qt Includes
#include <QtGui/QWidget>

// Project Includes
#include "../../Widgets/DraggableLabel.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "StateIcon/StateIconController.h"


namespace Ui {
    class TopUICreator;
}

class TopUICreator : public QWidget {
        Q_OBJECT
    public:
        TopUICreator( QWidget *parent = 0 );
        ~TopUICreator();
        void setLogoEnabled( bool enabled );
        void initStateDisplay( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory,
                               RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary );

    signals:
        void exitClicked();
        void dragging( QPoint distance );
        void doubleClicked();

    public slots:
        void exitClickedSlot();
        void draggingSlot( QPoint distance );
        void logoDoubleClicked();

    public:
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );

    protected:
        void changeEvent( QEvent *e );
        void configureLogo();
        void configureExitButton();
        void configureStatusDisplay( QWidget *parent );
        void mousePressEvent ( QMouseEvent * ev ) ;
        void mouseMoveEvent ( QMouseEvent * ev ) ;
        void mouseReleaseEvent ( QMouseEvent * ev ) ;
        void mouseDoubleClickEvent ( QMouseEvent * ev ) ;
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );
        void emitMainUIRegionFocusedChangedEvent();

    private:
        Ui::TopUICreator *_ui;
        RedBullPlayer::Widgets::DraggableLabel *_logoLabel;
        QImage _topLineImage;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        RedBullPlayer::Modules::TopUI::StateIconController* _stateController;
};

#endif // TOPUICREATOR_H
