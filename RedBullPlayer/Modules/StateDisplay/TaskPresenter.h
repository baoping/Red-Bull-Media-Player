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
#ifndef TASKPRESENTER_H
#define TASKPRESENTER_H

//QT Includes
#include <QObject>
#include <QList>
#include <QTimer>

// Project Includes
#include "../../Interfaces/StateDisplay/ITaskPresenter.h"
#include "../../Interfaces/StateDisplay/ITaskViewFactory.h"
#include "../../Interfaces/StateDisplay/IStateView.h"
#include "../../Interfaces/StateDisplay/ITaskModel.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class TaskPresenter : public ITaskPresenter {
                    Q_OBJECT

                public:
                    TaskPresenter( IStateView* view, ITaskModel* model, ITaskViewFactory* taskViewFactory, QObject* parent = 0 );
                    virtual ~TaskPresenter();
                    virtual QUuid guid() const {
                        return _guid;
                    }

                private:
                    QUuid _guid;
                    IStateView* _view;
                    ITaskModel* _model;
                    ITaskViewFactory* _taskViewFactory;
                    QTimer* _viewTimer;

                    void bootstrap();
                    void removeView( QUuid taskId );

                    virtual void showView( int x, int y );
                    virtual void hideView();
                    virtual bool isViewVisible();

                private slots:
                    void taskAdded( QUuid taskId );
                    void taskRemoved( QUuid taskId );
                    void taskStarted( QUuid taskId );
                    void taskProgress( QUuid taskId, int progress );
                    void cancelClicked( QUuid taskId );
                    void cancelAllClicked();

                    void viewTimerTimeout();
                    void mouseEnterView();
                    void mouseLeaveView();
            };
        }
    }
}

#endif // TASKPRESENTER_H
