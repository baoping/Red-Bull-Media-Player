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
#include "AboutDetailViewModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QTimer>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Constants.h"
#include "../../Events/EventTypes.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/Playlist/PlaylistNextItemSelectedEvent.h"
#include "../../Events/Playlist/PlaylistPreviousItemSelectedEvent.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::AboutDetailView;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Widgets;

AboutDetailViewModule::AboutDetailViewModule() : IAboutDetailView() {
}

void AboutDetailViewModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new AboutDetailViewUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    _region->widget()->setStyleSheet( "background-color: transparent" );
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
    Aggregator::instance()->registerListener( FunctionSelected, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    connect( _ui, SIGNAL( emitMainRegionFocusChanged() ), this, SLOT( emitMainRegionFocusChangedSlot() ) );
    _closeButton = new Button( _region->widget(),
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN,
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + "active" + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN,
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN );
    connect( _closeButton, SIGNAL( clicked() ), this, SLOT( exitClickedSlot() ) );
    _closeButton->setClickDownDistance( 0 );
    _forceToShowNextTime = false;
}

void AboutDetailViewModule::SetupModuleConnections() {
    resolveStyleFactory();
    setupUiStyle();
}

bool AboutDetailViewModule::event ( QEvent * e ) {
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
        updateGeometry();
    }

    return false;
}

void AboutDetailViewModule::setVisibility( bool visible ) {
    if( visible ) {
        updateGeometry();
        _region->widget()->show();
        CentralFocusChangedEvent ce( CentralFocusChangedEvent::ABOUT_DETAIL );
        Aggregator::instance()->sendEvent( &ce );
    } else {
        _region->widget()->hide();
    }
}

void AboutDetailViewModule::updateGeometry() {
    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( playlistRegion != NULL ) {
        _region->widget()->move( playlistRegion->x() + playlistRegion->width() / 4, playlistRegion->y() );
        _region->widget()->resize( playlistRegion->width() / 2, playlistRegion->height() );
        _closeButton->move( _region->widget()->width() - _closeButton->width() - Constants::NEWS_DETAIL_VIEW_CLOSE_BTN_RIGHT, Constants::NEWS_DETAIL_VIEW_CLOSE_BTN_TOP );
    }
}

void AboutDetailViewModule::onPlaylistItemSelected( PlaylistItemSelectedEvent* e ) {
    if( e->playlistItem() == NULL || e->playlistItem()->getMedia() == NULL ) {
        return;
    }

    IMedia *media = e->playlistItem()->getMedia();

    if( e->playlistItem()->getMedia()->mediaType().name() == "about" ) {
        _forceToShowNextTime = true;
        _ui->setTitle( media->title() );
        _ui->setText( media->description() );
        setVisibility( true );
    }
}

void AboutDetailViewModule::onFunctionSelected( FunctionSelectedEvent* e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "FunctionTitle: " + e->functionTitle() );

    if( e->functionTitle() == Constants::FUNCTION_SELECT_ABOUT_TITLE && _forceToShowNextTime ) {
        setVisibility( true );
        QTimer::singleShot( 750, this, SLOT( updateGeometry() ) );
    } else {
        setVisibility( false );
    }
}

void AboutDetailViewModule::onKeyPressed( KeyPressedEvent *event ) {
    if( _currentCentralWidgetType != CentralFocusChangedEvent::ABOUT_DETAIL || !_hasMainFocus ) {
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

void AboutDetailViewModule::exitClickedSlot() {
    _forceToShowNextTime = false;
    _ui->setTitle( "" );
    _ui->setText( "" );
    setVisibility( false );
    CentralFocusChangedEvent ce( CentralFocusChangedEvent::ABOUT_PLAYLIST );
    Aggregator::instance()->sendEvent( &ce );
}

void AboutDetailViewModule::emitMainRegionFocusChangedSlot() {
    if( !_hasMainFocus ) {
        MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
        Aggregator::instance()->sendEvent( &e );
    }
}

void AboutDetailViewModule::onCentralFocusChanged( CentralFocusChangedEvent *event ) {
    _currentCentralWidgetType = event->widgetType();
}

void AboutDetailViewModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTPLAYLISTCREATOR, Q_FUNC_INFO );

    if( event->uiRegion() == MainUIRegionFocusChangedEvent::CENTRAL ) {
        _hasMainFocus = true;
    } else {
        _hasMainFocus = false;
    }
}

void AboutDetailViewModule::resolveStyleFactory() {
    if( _ui == NULL )
        return;

    _ui->resolveStyleFactory();
}

void AboutDetailViewModule::setupUiStyle() {
    if( _ui == NULL )
        return;

    _ui->setupStyle();
}

Q_EXPORT_PLUGIN2( AboutDetailViewModule, RedBullPlayer::Modules::AboutDetailView::AboutDetailViewModule )
