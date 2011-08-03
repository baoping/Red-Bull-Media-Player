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
#include "ContentManagementModule.h"

//Qt includes
#include <QtPlugin>

//Project includes
#include "../../Events/RssFeed/RssFeedParseRequestEvent.h"
#include "../../Events/RssFeed/RssFeedParseResponseEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../Events/StateDisplay/RequestHoverBoxChangeVisibilityEvent.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Modules::StyleFactory;

ContentManagementModule::ContentManagementModule() : IContentManagement() {
    _internetAvailable = false;
    _allowAutomaticDownload = false;
}

void ContentManagementModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
}

void ContentManagementModule::SetupModuleConnections() {
    resolveMediaLibrary();
    resolveStyleFactory();
    bootstrapView( _styleFactory );
    _allowAutomaticDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    _podCastPoller = new PodCastPoller( this );
    _podCastCache = new PodCastCache( _mediaLibrary, this );
    _podCastWorker = new PodCastWorker( _mediaLibrary, _styleFactory, _podCastCache, this );
    connect( _podCastPoller,
             SIGNAL( rssFeedRetrieved( RedBullPlayer::Tools::RSS::IRssFeed* ) ),
             _podCastCache,
             SLOT( handleRssFeed( RedBullPlayer::Tools::RSS::IRssFeed* ) ) );
    connect( _podCastCache,
             SIGNAL( parseStarted() ),
             _podCastWorker,
             SLOT( parseStarted() ) );
    connect( _podCastCache,
             SIGNAL( parseFinished() ),
             _podCastWorker,
             SLOT( parseFinished() ) );
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
    Aggregator::instance()->registerListener( HoverBoxChangeVisibility, this );
    Aggregator::instance()->registerListener( ShellWidgetMoved, this );
    Aggregator::instance()->registerListener( UserSettingsChanged, this );
}

void ContentManagementModule::bootstrapView( IStyleFactory *styleFactory ) {
    _view = new ContentManagementUI( _shell );
    _contentManagementViewFactory = new ContentManagementViewFactory( styleFactory, _view, this );
    _contentManagementPresenter = new ContentManagementPresenter( _contentManagementViewFactory,
            _view, styleFactory, this );
}

bool ContentManagementModule::event( QEvent* event ) {
    if ( event->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( event ) );
    }

    if ( event->type() == HoverBoxChangeVisibility ) {
        onHoverBoxChangeVisibility( static_cast<HoverBoxChangeVisibilityEvent*>( event ) );
    }

    if( event->type() == ShellWidgetMoved ) {
        onShellWidgetMoved( static_cast<ShellWidgetMovedEvent*>( event ) );
    }

    if( event->type() == UserSettingsChanged ) {
        onUserSettingsChanged( static_cast<UserSettingsChangedEvent*>( event ) );
    }

    return false;
}

void ContentManagementModule::onShellWidgetMoved( ShellWidgetMovedEvent *e ) {
    if( _contentManagementPresenter->isViewVisible() ) {
        RequestHoverBoxChangeVisibilityEvent e( TaskPurpose::DOWNLOAD, true );
        Aggregator::instance()->sendEvent( &e );
    }
}

void ContentManagementModule::onHoverBoxChangeVisibility( HoverBoxChangeVisibilityEvent* e )  {
    if ( e->statusGroup() == TaskPurpose::DOWNLOAD ) {
        if ( e->visible() ) {
            _contentManagementPresenter->showView( e->xGlobalPosition(), e->yGlobalPosition() );
        } else {
            _contentManagementPresenter->hideView();
        }
    }
}

void ContentManagementModule::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent *e ) {
    _internetAvailable = e->internetAvailable();
    resolveConnectivityChanged();
}

void ContentManagementModule::onUserSettingsChanged( UserSettingsChangedEvent *e ) {
    _allowAutomaticDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    resolveConnectivityChanged();
}

void ContentManagementModule::resolveConnectivityChanged() {
    if ( _internetAvailable && _allowAutomaticDownload ) {
        if ( !_podCastPoller->isPolling() && _podCastPoller->canStart() ) {
            _podCastPoller->startPoll( ConfigManager::instance()->podCastPollInterval() );
        }
    } else {
        if ( _podCastPoller->isPolling() ) {
            _podCastPoller->stopPoll();
        }
    }
}


void ContentManagementModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}


void ContentManagementModule::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}


Q_EXPORT_PLUGIN2( ContentManagementModule, RedBullPlayer::Modules::ContentManagement::ContentManagementModule );
