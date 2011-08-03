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
#ifndef INTERNETAVAILABILITYPRESENTER_H
#define INTERNETAVAILABILITYPRESENTER_H

#include <QObject>
#include <QTimer>

#include "../../Interfaces/StateDisplay/IInternetAvailabilityPresenter.h"
#include "../../Interfaces/StateDisplay/IStateView.h"
#include "../../Interfaces/StateDisplay/IInternetAvailabilityModel.h"
#include "TaskViewFactory.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class InternetAvailabilityPresenter : public IInternetAvailabilityPresenter {
                    Q_OBJECT
                public:
                    InternetAvailabilityPresenter( IStateView* view, IInternetAvailabilityModel* model, ITaskViewFactory* taskViewFactory, QObject *parent = 0 );
                    virtual ~InternetAvailabilityPresenter() {}

                    virtual void handleInternetAvailabilityChanged( bool internetAvailable );
                    virtual QUuid guid() const {
                        return _guid;
                    }

                private:
                    QUuid _guid;
                    QUuid _viewGuid;
                    IStateView* _view;
                    IInternetAvailabilityModel* _model;
                    ITaskViewFactory* _taskViewFactory;
                    QTimer* _viewTimer;

                    void bootstrap();
                    void removeView( QUuid taskId );

                    virtual void showView( int x, int y );
                    virtual void hideView();
                    virtual bool isViewVisible();

                private slots:
                    void viewTimerTimeout();
                    void mouseEnterView();
                    void mouseLeaveView();
            };
        }
    }
}

#endif // INTERNETAVAILABILITYPRESENTER_H
