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
#ifndef STATEICONCONTROLLER_H
#define STATEICONCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include <QHash>

#include "StateIconModel.h"
#include "StateIconPresenter.h"
#include "StateDisplayButtonFactory.h"
#include "DiskSpaceModel.h"
#include "DiskSpacePresenter.h"
#include "ContentManagementModel.h"
#include "ContentManagementPresenter.h"
#include "InternetAvailabilityModel.h"
#include "InternetAvailabilityPresenter.h"

#include "../../../Widgets/StatusDisplayButton.h"
#include "../../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"
#include "../../../Events/StateDisplay/RequestHoverBoxChangeVisibilityEvent.h"
#include "../../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../../Events/ShellWidgetMovedEvent.h"
#include "../../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class StateIconController : public QObject {
                    Q_OBJECT
                public:
                    StateIconController( QWidget* buttonParent, QBoxLayout* buttonContainer, QObject *parent = 0 );

                    void init( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory,
                               RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary );

                protected:
                    bool event( QEvent* event );

                    void onHoverBoxChangeVisibility( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent* e );
                    void onRequestHoverBoxChangeVisibility ( RedBullPlayer::Events::RequestHoverBoxChangeVisibilityEvent* e );

                private:
                    QWidget* _buttonParent;
                    QBoxLayout* _buttonContainer;

                    StateDisplayButtonFactory* _buttonFactory;

                    RedBullPlayer::Widgets::StatusDisplayButton* _diskSpaceButton;
                    RedBullPlayer::Widgets::StatusDisplayButton* _ioButton;
                    RedBullPlayer::Widgets::StatusDisplayButton* _downloadButton;
                    RedBullPlayer::Widgets::StatusDisplayButton* _updateButton;
                    RedBullPlayer::Widgets::StatusDisplayButton* _internetAvailabilityButton;

                    DiskSpaceModel* _diskSpaceModel;
                    StateIconModel* _ioModel;
                    ContentManagementModel* _contentModel;
                    InternetAvailabilityModel *_internetAvailabilityModel;
                    StateIconModel* _updateModel;

                    StateIconPresenter* _ioPresenter;
                    ContentManagementPresenter* _contentPresenter;
                    StateIconPresenter* _updatePresenter;
                    DiskSpacePresenter* _diskSpacePresenter;
                    InternetAvailabilityPresenter *_internetAvailabilityPresenter;

            };
        }
    }
}

#endif // STATEICONCONTROLLER_H
