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
#ifndef TASKVIEWUI_H
#define TASKVIEWUI_H

//QT includes
#include <QMovie>
#include <QtGui/QWidget>
#include <QPair>
#include <QUuid>

//Project includes
#include "../../Interfaces/StateDisplay/ITaskView.h"
#include "../../Interfaces/TaskManager/ITask.h"
#include "../../Widgets/CommandButton.h"
#include "../../Interfaces/TaskManager/Types.h"

namespace Ui {
    class TaskViewUI;
}

class TaskViewUI : public RedBullPlayer::Modules::StateDisplay::ITaskView {
        Q_OBJECT
    public:
        TaskViewUI( QWidget *parent = 0 );
        ~TaskViewUI();
        virtual void setTaskInfo( RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo );
        virtual RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo() const {
            return _taskInfo;
        }
        void activate();
        void deactivate();

        virtual void setText( QString text );
        virtual void setProgressDisplayText( QString text );

        virtual void showCancelButton();
        virtual void hideCancelButton();

        virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle );

    public slots:
        void cancelClickedSlot();

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent * );

        void initTaskViewSize();
        void initCancelButton();
        void initCircleProgessAnimation();
        void setNameLabelText();
        void startCircleAnimation();
        void stopCircleAnimation();


    private:
        Ui::TaskViewUI *_ui;
        RedBullPlayer::Modules::TaskManager::TaskInfo _taskInfo;
        RedBullPlayer::Widgets::CommandButton *_cancelButton;
        QMovie *_circleAnimation;
};

#endif // TASKVIEWUI_H
