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
#include "XmlNewsFeedModule.h"

// Qt includes
#include <QtPlugin>
#include <QDebug>

// Project includes
#include "../../Container/Plugin.h"
#include "../../PlayerShell/Shell.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistGenerationFinishedEvent.h"
#include "../../Events/RssFeed/RssFeedPlaylistCreationRequestEvent.h"
#include "../../Events/RssFeed/RssFeedParseRequestEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::XmlNewsFeed;
using namespace RedBullPlayer::Modules::InternetConnectionChecker;
using namespace RedBullPlayer::Modules::MediaLibrary;


XmlNewsFeedModule::XmlNewsFeedModule() : IXmlNewsFeed() {
}

void XmlNewsFeedModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    _internetAvailable = false;
    _allowDownload = false;
}

void XmlNewsFeedModule::SetupModuleConnections() {
    resolveMediaLibrary();
    _allowDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    _currentFeed = 0;
    _currentFeedItemIndex = 0;
    _newFeedItem = false;
    _lastRequestGuid = RedBullPlayer::Tools::UuidCreator::create();
    _checkTimer = new QTimer( this );
    _checkTimer->setInterval( 30000 );
    connect( _checkTimer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );
    _parserQueue = new RssFeedParserQueue( this );
    _parserEventBridge = new RssFeedParserEventBridge( _parserQueue, this );
    _converter = new RssFeedToNewsConverter( _mediaLibrary, this );
    connect( _converter,
             SIGNAL( convertFinished( RedBullPlayer::Modules::MediaLibrary::INews*, QList<RedBullPlayer::Modules::MediaLibrary::INewsImage*>* ) ),
             this,
             SLOT( saveNewsToDb( RedBullPlayer::Modules::MediaLibrary::INews*, QList<RedBullPlayer::Modules::MediaLibrary::INewsImage*>* ) ) );
    RssFeedPlaylistCreationRequestEvent ev( RedBullPlayer::Tools::UuidCreator::create() );
    Aggregator::instance()->sendEvent( &ev );

    if ( ConfigManager::instance()->rssFeedAvailable() ) {
        Aggregator::instance()->registerListener( RssFeedParseResponse, this );
        Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
        Aggregator::instance()->registerListener( UserSettingsChanged, this );
    }
}

void XmlNewsFeedModule::onTimer() {
    doRequest();
}

void XmlNewsFeedModule::doRequest() {
    RssFeedParseRequestEvent event( _lastRequestGuid, ConfigManager::instance()->getRssFeedUrl().toString() );
    Aggregator::instance()->sendEvent( &event );
}

bool XmlNewsFeedModule::resolveMediaLibrary() {
    bool success = false;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
        success = true;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return success;
}

void XmlNewsFeedModule::saveRssFeedToDb( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed ) {
    _currentFeed = rssFeed;
    _currentFeedItemIndex = 0;
    _newFeedItem = false;
    processNextRssFeedItem();
}

void XmlNewsFeedModule::processNextRssFeedItem() {
    if ( _currentFeedItemIndex < _currentFeed->itemCount() ) {
        INews* news = _mediaLibrary->mediaDB()->newsRepository()->getNewsByExternalGuid( _currentFeed->itemAt( _currentFeedItemIndex )->guid() );

        if ( news == 0 ) {
            _newFeedItem = true;
            _converter->convert( _currentFeed->itemAt( _currentFeedItemIndex ) );
        } else {
            _currentFeedItemIndex++;
            processNextRssFeedItem();
        }
    } else {
        if ( _newFeedItem ) {
            RssFeedPlaylistCreationRequestEvent ev( RedBullPlayer::Tools::UuidCreator::create() );
            Aggregator::instance()->sendEvent( &ev );
        }
    }
}

void XmlNewsFeedModule::saveNewsToDb( RedBullPlayer::Modules::MediaLibrary::INews* news, QList<RedBullPlayer::Modules::MediaLibrary::INewsImage*>* newsImages ) {
    Error err;
    _mediaLibrary->mediaDB()->newsRepository()->saveNews( *news, &err );
    Error::raise( &err );
    delete news;

    while ( !newsImages->isEmpty() ) {
        INewsImage* image = newsImages->takeFirst();
        _mediaLibrary->mediaDB()->newsRepository()->saveNewsImage( *image , &err );
        Error::raise( &err, true );
        delete image;
    }

    delete newsImages;
    _currentFeedItemIndex++;
    processNextRssFeedItem();
}

bool XmlNewsFeedModule::event ( QEvent * e ) {
    if( e->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( e ) );
    }

    if( e->type() == RssFeedParseResponse ) {
        onRssFeedParseResponse( static_cast<RssFeedParseResponseEvent*>( e ) );
    }

    if( e->type() == UserSettingsChanged ) {
        onUserSettingsChanged( static_cast<UserSettingsChangedEvent*>( e ) );
    }

    return false;
}

void XmlNewsFeedModule::onRssFeedParseResponse( RssFeedParseResponseEvent* ev ) {
    if ( ev->requestId() == _lastRequestGuid ) {
        if ( ConfigManager::instance()->alternativeNewsBehavior() ) {
            Error err;
            _mediaLibrary->mediaDB()->newsRepository()->clearCache( &err );
            Error::raise( &err, true );
        }

        saveRssFeedToDb( ev->rssFeed() );
    }
}

void XmlNewsFeedModule::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent* ev ) {
    _internetAvailable = ev->internetAvailable();
    resolveConnectivityChanged();
}

void XmlNewsFeedModule::onUserSettingsChanged( UserSettingsChangedEvent *e ) {
    _allowDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    resolveConnectivityChanged();
}

void XmlNewsFeedModule::resolveConnectivityChanged() {
    if ( _internetAvailable && _allowDownload && !_checkTimer->isActive() )
        _checkTimer->start();

    if ( !( _internetAvailable && _allowDownload ) && _checkTimer->isActive() )
        _checkTimer->stop();
}



Q_EXPORT_PLUGIN2( XmlNewsFeedModule, RedBullPlayer::Modules::XmlNewsFeed::XmlNewsFeedModule )
