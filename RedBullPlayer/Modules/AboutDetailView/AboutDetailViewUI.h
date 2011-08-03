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
#ifndef ABOUTDETAILVIEWUI_H
#define ABOUTDETAILVIEWUI_H

#include <QtGui/QWidget>

//Project includes
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Widgets/TextBrowser.h"

namespace Ui {
    class AboutDetailViewUI;
}

class AboutDetailViewUI : public QWidget {
        Q_OBJECT
    public:
        AboutDetailViewUI( QWidget *parent = 0 );
        ~AboutDetailViewUI();

        void setTitle( QString title );
        void setText( QString text );
        void resolveStyleFactory();
        void resolveMediaLibrary();
        void setupStyle();

    signals:
        void emitMainRegionFocusChanged();

    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );
        void setupTitleStyle();

    private:
        Ui::AboutDetailViewUI *_ui;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
        RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
        RedBullPlayer::Widgets::TextBrowser* _browser;
};

#endif // ABOUTDETAILVIEWUI_H
