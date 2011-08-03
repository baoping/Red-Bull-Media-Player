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
#ifndef HOVERBOXUI_H
#define HOVERBOXUI_H

#include <QtGui/QWidget>
#include <QUuid>
#include <QHash>
#include <QBoxLayout>

//Project includes
#include "../../Interfaces/StateDisplay/IStateView.h"
#include "../../Interfaces/StateDisplay/ITaskView.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class HoverBoxUI;
}

class HoverBoxUI : public RedBullPlayer::Modules::StateDisplay::IStateView {
        Q_OBJECT
    public:
        HoverBoxUI( QString cancelAllImageUrl, QString cancelAllMouseOverImageUrl, QWidget *parent = 0 );
        ~HoverBoxUI();

        virtual QUuid guid() const;
        virtual void append( RedBullPlayer::Modules::StateDisplay::ITaskView* taskView );
        virtual void remove( QUuid taskId );
        virtual void activate( QUuid taskId );
        virtual void updateText( QUuid taskId, QString text );
        virtual void updateProgressText( QUuid taskId, QString text );

        virtual void show( int x, int y );
        virtual void hide();
        virtual void showCancelAll();
        virtual void hideCancelAll();
        virtual void setTitle( QString title );
        virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle );

    protected:
        void changeEvent( QEvent *e );
        void enterEvent( QEvent * );
        void leaveEvent( QEvent * );
        void paintEvent( QPaintEvent *e );

        void setWindowAttributes();
        void initCancelAllButton();
        void initHoverBoxSize();
        void initLayout();
        void setTaskViewContentHeight( int newHeight );
        void setTaskViewContentWidth( int newHeight );

    private:
        void styleScrollBar();

    private:
        QString _cancelAllImageUrl;
        QString _cancelAllMouseOverImageUrl;

        Ui::HoverBoxUI *_ui;
        QUuid _id;
        QHash<QUuid, RedBullPlayer::Modules::StateDisplay::ITaskView*> _taskItems;
        RedBullPlayer::Widgets::CommandButton *_cancelAllButton;
        int _taskViewContentAreaMaximumHeight;
        int _taskViewContentAreaWidthScrollBarVisible;
        int _taskViewContentAreaWidthScrollBarNotVisible;
        QBoxLayout *_taskViewContentLayout;
};

#endif // HOVERBOXUI_H
