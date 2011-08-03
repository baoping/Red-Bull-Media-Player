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
#ifndef CONTENTMANAGEMENTPRESENTER_H
#define CONTENTMANAGEMENTPRESENTER_H

#include <QObject>
#include <QTimer>

#include "ContentManagementUI.h"
#include "ContentManagementViewFactory.h"
#include "ContentManagementEventBridge.h"
#include "ContentManagementModel.h"
#include "../../Interfaces/ContentManagement/IContentListView.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentManagementPresenter : public QObject {
                    Q_OBJECT
                public:
                    ContentManagementPresenter( ContentManagementViewFactory *viewFactory,
                                                ContentManagementUI *view,
                                                RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory,
                                                QObject *parent = 0 );

                    void showView( int x, int y );
                    void hideView();
                    bool isViewVisible();

                protected slots:
                    void viewTimerTimeout();
                    void mouseEnterView();
                    void mouseLeaveView();

                protected:
                    void bootstrapView();
                    void bootstrapTabs();

                private:
                    ContentManagementViewFactory *_viewFactory;
                    ContentManagementUI *_view;
                    ContentManagementEventBridge *_eventBridge;
                    ContentManagementModel *_model;
                    QTimer *_viewTimer;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;


            };
        }
    }
}

#endif // CONTENTMANAGEMENTPRESENTER_H
