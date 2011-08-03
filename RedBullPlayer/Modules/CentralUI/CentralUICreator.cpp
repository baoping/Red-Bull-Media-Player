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
#include "CentralUICreator.h"
#include "ui_CentralUICreator.h"

//Qt Includes
#include <QDebug>
#include <QResizeEvent>
#include <QPainter>

// Project Includes
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "../../ToolLib/SimpleLogger.h"
#include "SubWindow.h"
#include "SubWidget.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

CentralUICreator::CentralUICreator( Shell* shell, QWidget *parent ) :
    QStackedWidget( parent ),
    _ui( new Ui::CentralUICreator ) {
    Q_ASSERT( shell != 0 );
    _shell = shell;
    _ui->setupUi( this );

    //StandByRegion fruher
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "PlayerRegion", _ui->playerPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "PlaylistRegion", _ui->playListPage ) );

    QWidget *newsWidget = new QWidget( parent );
    newsWidget->hide();
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "NewsDetailViewRegion", newsWidget ) );

    QWidget *aboutWidget = new QWidget( parent );
    aboutWidget->hide();
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "AboutDetailViewRegion", aboutWidget ) );

    QWidget *settingsWidget = new QWidget( parent );
    settingsWidget->hide();
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "SettingsDetailViewRegion", settingsWidget ) );

    SubWindow *controlsSubWindow = new SubWindow( parent, _shell );
    controlsSubWindow->hide();
    controlsSubWindow->setLayout( new QGridLayout( controlsSubWindow ) );
    controlsSubWindow->layout()->setMargin( 0 );
    controlsSubWindow->layout()->setSpacing( 0 );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "ControllBarRegion", controlsSubWindow ) );

    SubWindow *chapterSelect = new SubWindow( parent, _shell );
    chapterSelect->hide();
    chapterSelect->setLayout( new QGridLayout( chapterSelect ) );
    chapterSelect->layout()->setMargin( 0 );
    chapterSelect->layout()->setSpacing( 0 );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "ChapterSelectRegion", chapterSelect ) );

    SubWindow *playerCloseButtonRegion = new SubWindow( parent, _shell );
    playerCloseButtonRegion->setStyleSheet( "background-color: black;" ); //Hack so Player Close Button can be displayed color mandatory
    playerCloseButtonRegion->hide();
    playerCloseButtonRegion->setLayout( new QGridLayout( playerCloseButtonRegion ) );
    playerCloseButtonRegion->layout()->setMargin( 0 );
    playerCloseButtonRegion->layout()->setSpacing( 0 );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "PlayerCloseButtonRegion", playerCloseButtonRegion ) );

    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::CENTRAL;
    handlePlayerVisibilityChanged( false );
}

CentralUICreator::~CentralUICreator() {
    delete _ui;
}

void CentralUICreator::setCurrentWidget( CentralUIWidgetChangedEvent::CentralUIWidgets widget ) {
    switch( widget ) {
        case CentralUIWidgetChangedEvent::VIDEOPLAYER:
            handlePlayerVisibilityChanged( true );
            break;

        case CentralUIWidgetChangedEvent::STANDBY:
            handlePlayerVisibilityChanged( false );
            break;
    }
}

void CentralUICreator::changeEvent( QEvent *e ) {
    QStackedWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void CentralUICreator::resizeEvent( QResizeEvent *event ) {
    QStackedWidget::resizeEvent( event );
    CentralUIRegionResizedEvent e( event->size().width(),
                                   event->size().height() );
    Aggregator::instance()->sendEvent( &e );
}

void CentralUICreator::enterEvent( QEvent */*e*/ ) {
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
    Aggregator::instance()->sendEvent( &e );
}

void CentralUICreator::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    _hasMainUIRegionFocus = ( _mainUIRegionGroup == mainUIRegion );

    if( _currentFocusedCentralWidgetType != CentralFocusChangedEvent::ABOUT_PLAYLIST ) {
        if( _hasMainUIRegionFocus ) {
            _ui->playListPage->setStyleSheet( Constants::CENTRAL_UI_PLAYLIST_BACKGROUND_FOCUSED_COLOR );
        } else {
            _ui->playListPage->setStyleSheet( Constants::CENTRAL_UI_PLAYLIST_BACKGROUND_UNFOCUSED_COLOR );
        }

        _ui->playListPage->update();
    }
}

void CentralUICreator::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    _currentFocusedCentralWidgetType = widgetType;
}

void CentralUICreator::handlePlayerVisibilityChanged( bool visible ) {
    setCurrentIndex( visible ? 0 : 1 );
}

