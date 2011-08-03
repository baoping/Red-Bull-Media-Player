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
#ifndef CONTENTMANAGEMENTUI_H
#define CONTENTMANAGEMENTUI_H

#include <QtGui/QWidget>

#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class ContentManagementUI;
}

class ContentManagementUI : public QWidget {
        Q_OBJECT
    public:
        ContentManagementUI( QWidget *parent = 0 );
        ~ContentManagementUI();

        void show( int x, int y );
        void hide();
        void setTabTitles( QString availableTitle, QString downloadedTitle, QString skippedTitle );
        void addAvailableListView( QWidget *listView );
        void addDownloadedListView( QWidget *listView );
        void addSkippedListView( QWidget *listView );
        void setTabTitlesStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *titleStyle );

    signals:
        void mouseEnter();
        void mouseLeave();

    protected:
        void changeEvent( QEvent *e );
        void enterEvent( QEvent * );
        void leaveEvent( QEvent * );
        void paintEvent( QPaintEvent *e );

        void setWindowAttributes();

    private:
        Ui::ContentManagementUI *_ui;
};

#endif // CONTENTMANAGEMENTUI_H
