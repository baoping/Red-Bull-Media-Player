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
#ifndef AVAILABLEDOWNLOADVIEW_H
#define AVAILABLEDOWNLOADVIEW_H

#include <QtGui/QWidget>

#include "../../Interfaces/ContentManagement/IContentItemView.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class AvailableDownloadView;
}

class AvailableDownloadView : public RedBullPlayer::Modules::ContentManagement::IContentItemView {
        Q_OBJECT
    public:
        AvailableDownloadView( QString guid, QWidget *parent = 0 );
        ~AvailableDownloadView();

        virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle );
        virtual void setTitle( QString title );
        virtual void setSize( QString sizeString );
        virtual void setToolTipText( QString toolTip );

        void onDownloadStarted();
        void setProgress( int progressValue );
        void onDownloadCanceled();

    signals:
        void downloadSignal( QString guid );
        void skipSignal( QString guid );
        void cancelSignal( QString guid );

    protected slots:
        void downloadClicked();
        void skipClicked();
        void cancelClicked();

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent * );

    private:
        Ui::AvailableDownloadView *_ui;
        RedBullPlayer::Widgets::CommandButton *_downloadButton;
        RedBullPlayer::Widgets::CommandButton *_skipButton;
        RedBullPlayer::Widgets::CommandButton *_cancelButton;
        QString _titleText;
        int _labelsMarginRight;
};

#endif // AVAILABLEDOWNLOADVIEW_H
