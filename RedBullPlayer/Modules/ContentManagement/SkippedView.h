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
#ifndef SKIPPEDVIEW_H
#define SKIPPEDVIEW_H

#include <QtGui/QWidget>

#include "../../Interfaces/ContentManagement/IContentItemView.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class SkippedView;
}

class SkippedView : public RedBullPlayer::Modules::ContentManagement::IContentItemView {
        Q_OBJECT
    public:
        SkippedView( QString guid, QWidget *parent = 0 );
        ~SkippedView();

        virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle );
        virtual void setTitle( QString title );
        virtual void setSize( QString sizeString );
        virtual void setToolTipText( QString toolTip );


    protected slots:
        void downloadClickedSlot();
        void deleteClickedSlot();

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent * );

    private:
        Ui::SkippedView *_ui;

        RedBullPlayer::Widgets::CommandButton *_downloadButton;
        RedBullPlayer::Widgets::CommandButton *_deleteButton;
        QString _titleText;

    signals:
        void downloadClicked( QString contentId );
        void deleteClicked( QString contentId );

};

#endif // SKIPPEDVIEW_H
