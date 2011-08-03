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
#ifndef DISKSPACEPRESENTER_H
#define DISKSPACEPRESENTER_H

//QT Includes
#include <QObject>
#include <QList>
#include <QTimer>

// Project Includes
#include "../../Interfaces/StateDisplay/IDiskSpacePresenter.h"
#include "../../Interfaces/StateDisplay/ITaskViewFactory.h"
#include "../../Interfaces/StateDisplay/IStateView.h"
#include "../../Interfaces/StateDisplay/IDiskSpaceModel.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {

            class DiskSpacePresenter : public IDiskSpacePresenter {
                    Q_OBJECT
                public:
                    DiskSpacePresenter( IStateView* view, IDiskSpaceModel* model, ITaskViewFactory* taskViewFactory, QObject *parent = 0 );
                    virtual ~DiskSpacePresenter();

                    virtual QUuid guid() const {
                        return _guid;
                    }

                private:
                    QUuid _guid;
                    QUuid _viewGuid;
                    IStateView* _view;
                    IDiskSpaceModel* _model;
                    ITaskViewFactory* _taskViewFactory;
                    QTimer* _viewTimer;

                    void bootstrap();
                    void removeView( QUuid taskId );

                    virtual void showView( int x, int y );
                    virtual void hideView();
                    virtual bool isViewVisible();

                private slots:
                    void cancelAllClicked();
                    void viewTimerTimeout();
                    void mouseEnterView();
                    void mouseLeaveView();
            };
        }
    }
}

#endif // DISKSPACEPRESENTER_H
