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
#ifndef NEWSDETAILVIEWUI_H
#define NEWSDETAILVIEWUI_H

#include <QtGui/QWidget>
#include <QDateTime>

//Project includes
#include "../../Widgets/TextBrowser.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Widgets/Button.h"

namespace Ui {
    class NewsDetailViewUI;
}

class NewsDetailViewUI : public QWidget {
        Q_OBJECT
    public:
        NewsDetailViewUI( QWidget *parent = 0 );
        ~NewsDetailViewUI();

        void setTitle( QString title );
        void setLink( QString url );
        void setDescription( QString description );
        void setText( QString text );
        void setPubDate( QString pubDate );
        void setNewsGuid( QUuid newsGuid );

        void resolveStyleFactory();
        void resolveMediaLibrary();

        void setupStyle();

    signals:
        void exitClicked();

    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );
        void setupTitleStyle();
        void setupPublishDateStyle();
        void setupLinkStyle();
        void setupDescriptionStyle();

    private:
        Ui::NewsDetailViewUI *_ui;

        RedBullPlayer::Widgets::Button *_closeButton;
        RedBullPlayer::Widgets::TextBrowser* _browser;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
        RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
        QString _linkStyle;
};

#endif // NEWSDETAILVIEWUI_H
