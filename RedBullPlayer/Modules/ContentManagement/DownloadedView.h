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
#ifndef DOWNLOADEDVIEW_H
#define DOWNLOADEDVIEW_H

#include <QtGui/QWidget>

#include "../../Interfaces/ContentManagement/IContentItemView.h"

namespace Ui {
    class DownloadedView;
}

class DownloadedView : public RedBullPlayer::Modules::ContentManagement::IContentItemView {
        Q_OBJECT
    public:
        DownloadedView( QString guid, QWidget *parent = 0 );
        ~DownloadedView();

        virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle );
        virtual void setTitle( QString title );
        virtual void setSize( QString sizeString );
        virtual void setToolTipText( QString toolTip );

        void setDate( QString dateString );

    protected:
        void changeEvent( QEvent *e );

    private:
        Ui::DownloadedView *_ui;
        QString _titleText;
};

#endif // DOWNLOADEDVIEW_H
