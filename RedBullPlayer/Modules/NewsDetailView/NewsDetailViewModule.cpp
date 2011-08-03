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
#include "NewsDetailViewModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Constants.h"
#include "../../Events/EventTypes.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/Playlist/PlaylistNextItemSelectedEvent.h"
#include "../../Events/Playlist/PlaylistPreviousItemSelectedEvent.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::NewsDetailView;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;

NewsDetailViewModule::NewsDetailViewModule() : INewsDetailView() {
    _fixedWidth = 0;
    _fixedHeight = 0;
}

void NewsDetailViewModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new NewsDetailViewUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    _region->widget()->setStyleSheet( "background-color: transparent" );
    connect( _ui, SIGNAL( exitClicked() ), this, SLOT( exitClickedSlot() ) );
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
    Aggregator::instance()->registerListener( FunctionSelected, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    _forceToShowNextTime = false;
}

void NewsDetailViewModule::SetupModuleConnections() {
    resolveStyleFactory();
    setupUiStyle();
    getFixedSize();
}

bool NewsDetailViewModule::event ( QEvent * e ) {
    if( e->type() == PlaylistItemSelected ) {
        onPlaylistItemSelected( static_cast<PlaylistItemSelectedEvent*>( e ) );
    }

    if( e->type() == FunctionSelected ) {
        onFunctionSelected( static_cast<FunctionSelectedEvent*>( e ) );
    }

    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == CentralUIRegionResized ) {
        onCentralUIRegionResized( static_cast<CentralUIRegionResizedEvent*>( e ) );
    }

    return false;
}

void NewsDetailViewModule::setVisibility( bool visible ) {
    if( visible ) {
        updateGeometry();
        _region->widget()->show();
        CentralFocusChangedEvent ce( CentralFocusChangedEvent::NEWS_DETAIL );
        Aggregator::instance()->sendEvent( &ce );
    } else {
        _region->widget()->hide();
    }
}

void NewsDetailViewModule::updateGeometry() {
    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( playlistRegion != NULL ) {
        int width = _fixedWidth;
        int height = _fixedHeight;
        int widthProportional = ( int ) playlistRegion->width() * Constants::NEWSDETAILVIEW_WIDTH_PROCENT_PLAYLISTREGION / 100;
        int heightProportional = ( int ) playlistRegion->height() * Constants::NEWSDETAILVIEW_HEIGHT_PROCENT_PLAYLISTREGION / 100;

        if( width < widthProportional )
            width = widthProportional;
        else if( width > playlistRegion->width() )
            width = playlistRegion->width();

        if( height < heightProportional )
            height = heightProportional;
        else if( height > playlistRegion->height() )
            height = playlistRegion->height();

        int left = ( playlistRegion->x() + playlistRegion->width() / 2 ) - ( width / 2 );
        int top = playlistRegion->y();
        _region->widget()->move( left, top );
        _region->widget()->resize( width, height );
    }
}

void NewsDetailViewModule::onCentralUIRegionResized( CentralUIRegionResizedEvent *e ) {
    updateGeometry();
}

void NewsDetailViewModule::onPlaylistItemSelected( PlaylistItemSelectedEvent* e ) {
    if( e->playlistItem() == NULL || e->playlistItem()->getMedia() == NULL ) {
        //Error error;
        //error.setErrorMessage(tr("Cannot show news. Media object is NULL."));
        //Error::raise(&error);
        return;
    }

    IMedia *media = e->playlistItem()->getMedia();

    if( e->playlistItem()->getMedia()->mediaType().name() == "news" ) {
        _forceToShowNextTime = true;
        _ui->setTitle( media->title() );
        _ui->setLink( media->url() );
        _ui->setDescription( media->description() );
        _ui->setText( media->copyright() ); //DIRTY HACK
        _ui->setPubDate( media->artist() );
        _ui->setNewsGuid( media->guid() );
        setVisibility( true );
    }
}

void NewsDetailViewModule::onFunctionSelected( FunctionSelectedEvent* e ) {
    if( e->functionTitle() == Constants::FUNCTION_SELECT_NEWS_TITLE && _forceToShowNextTime ) {
        setVisibility( true );
        QTimer::singleShot( 750, this, SLOT( updateGeometry() ) );
    } else {
        setVisibility( false );
    }
}

void NewsDetailViewModule::onKeyPressed( KeyPressedEvent *event ) {
    if( _currentCentralWidgetType != CentralFocusChangedEvent::NEWS_DETAIL || !_hasMainFocus ) {
        return;
    }

    if( event->keyEvent()->key() == Qt::Key_Escape ) {
        exitClickedSlot();
    }

    if( event->keyEvent()->key() == Qt::Key_Right ) {
        PlaylistNextItemSelectedEvent e;
        Aggregator::instance()->sendEvent( &e );
    }

    if( event->keyEvent()->key() == Qt::Key_Left ) {
        PlaylistPreviousItemSelectedEvent e;
        Aggregator::instance()->sendEvent( &e );
    }
}

void NewsDetailViewModule::exitClickedSlot() {
    _forceToShowNextTime = false;
    _ui->setTitle( "" );
    _ui->setLink( "" );
    _ui->setDescription( "" );
    _ui->setText( "" );
    _ui->setPubDate( "" );
    setVisibility( false );
    CentralFocusChangedEvent ce( CentralFocusChangedEvent::NEWS_PLAYLIST );
    Aggregator::instance()->sendEvent( &ce );
}

void NewsDetailViewModule::onCentralFocusChanged( CentralFocusChangedEvent *event ) {
    _currentCentralWidgetType = event->widgetType();
}

void NewsDetailViewModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    if( event->uiRegion() == MainUIRegionFocusChangedEvent::CENTRAL ) {
        _hasMainFocus = true;
    } else {
        _hasMainFocus = false;
    }
}

void NewsDetailViewModule::resolveStyleFactory() {
    if( _ui == NULL )
        return;

    _ui->resolveStyleFactory();
}

void NewsDetailViewModule::resolveMediaLibrary() {
    if( _ui == NULL )
        return;

    _ui->resolveMediaLibrary();
}

void NewsDetailViewModule::setupUiStyle() {
    if( _ui == NULL )
        return;

    _ui->setupStyle();
}

void NewsDetailViewModule::getFixedSize() {
    QSize minScreenSize = getSmallestMonitorScreenSize();
    _fixedWidth = ( int ) minScreenSize.width() * Constants::NEWSDETAILVIEW_WIDTH_PROCENT_PLAYLISTREGION / 100;
    QWidget *topRegion = _shell->findRegion( "TopRegion" )->widget();
    QWidget *searchRegion = _shell->findRegion( "SearchRegion" )->widget();
    _fixedHeight = ( int )( minScreenSize.height() - topRegion->height() - searchRegion->height() - topRegion->height() )
                   * Constants::NEWSDETAILVIEW_HEIGHT_PROCENT_PLAYLISTREGION / 100;
}

QSize NewsDetailViewModule::getSmallestMonitorScreenSize() {
    QSize minSize = QSize( 100000, 100000 );

    for( int i = 0; i < QApplication::desktop()->numScreens(); i++ ) {
        if( minSize.width() > QApplication::desktop()->screenGeometry( i ).size().width() )
            minSize.setWidth( QApplication::desktop()->screenGeometry( i ).size().width() );

        if( minSize.height() > QApplication::desktop()->screenGeometry( i ).size().height() )
            minSize.setHeight( QApplication::desktop()->screenGeometry( i ).size().height() );
    }

    return minSize;
}

Q_EXPORT_PLUGIN2( NewsDetailViewModule, RedBullPlayer::Modules::NewsDetailView::NewsDetailViewModule )
