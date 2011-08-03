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
#ifndef CONTENTMANAGEMENTMODULE_H
#define CONTENTMANAGEMENTMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/ContentManagement/IContentManagement.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Events/ShellWidgetMovedEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"

//Module Includes
#include "PodCastPoller.h"
#include "PodCastCache.h"
#include "PodCastWorker.h"
#include "ContentManagementUI.h"
#include "ContentManagementPresenter.h"
#include "ContentManagementViewFactory.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentManagementModule : public IContentManagement {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::ContentManagement::IContentManagement )

                public:
                    ContentManagementModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "ContentManagement";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    bool event( QEvent* event );

                    void onHoverBoxChangeVisibility( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent* e );
                    void onShellWidgetMoved( RedBullPlayer::Events::ShellWidgetMovedEvent *e );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent *e );
                    void onUserSettingsChanged( RedBullPlayer::Events::UserSettingsChangedEvent *e );

                    void bootstrapView( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory );

                private:
                    void resolveMediaLibrary();
                    void resolveStyleFactory();
                    void resolveConnectivityChanged();

                    RedBullPlayer::PlayerShell::Shell* _shell;
                    RedBullPlayer::PlayerShell::Region* _region;

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;

                    bool _internetAvailable;
                    bool _allowAutomaticDownload;
                    PodCastPoller* _podCastPoller;
                    PodCastCache* _podCastCache;
                    PodCastWorker* _podCastWorker;

                    ContentManagementUI *_view;
                    ContentManagementPresenter *_contentManagementPresenter;
                    ContentManagementViewFactory *_contentManagementViewFactory;

            };
        }
    }
}

#endif // CONTENTMANAGEMENTMODULE_H
