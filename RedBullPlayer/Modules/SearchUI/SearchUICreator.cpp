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
#include "SearchUICreator.h"
#include "ui_SearchUICreator.h"

// QT Includes
#include <QDebug>
#include <QPainter>

#include <math.h>

// Project Includes
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/SearchChangedEvent.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Container.h"
#include "../../Container/Error.h"


using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

SearchUICreator::SearchUICreator( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::SearchUICreator ) {
    _ui->setupUi( this );
    _backgroundImage = QPixmap( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_BG_FILENAME );
    _backgroundImageSearchTextBox = QPixmap( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_SEARCHBOX_BG_FILENAME );
    _imageSearchBoxCover =  QPixmap( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_SEARCHBOX_COVER_FILENAME );
    _imageSearchBoxHead =  QPixmap( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_SEARCHBOX_HEAD_FILENAME );
    _imageGradientLine = QPixmap( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_GRADIENT_LINE );
    _grayLineWidthToTotalWidthRatio = 0.85;
    _hasMainUIRegionFocus = false;
    _searchEnabledForCurrentPlaylist = true;
    _searchGloballyEnabled = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::TOP;
    connect( _ui->_searchText, SIGNAL( textChanged( QString ) ),
             this, SLOT( searchTextChanged( QString ) ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, "Add Button to SearchUI" );
    _clearButton = new CommandButton( this,
                                      Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_SEARCHBOX_X );
    _clearButton->setMouseOverImage( Constants::SEARCH_UI_IMAGE_DIR + Constants::SEARCH_UI_SEARCHBOX_X_PRESSED );
    _clearButton->setVisible( false ); //hide on startup
    connect( _clearButton, SIGNAL( clicked() ), SLOT( clearButtonClicked() ) );
    _ui->horizontalLayout->insertWidget( 2, _clearButton );
    _ui->_searchText->setAttribute( Qt::WA_MacShowFocusRect, false );
}

SearchUICreator::~SearchUICreator() {
    delete _ui;
}


void SearchUICreator::clearButtonClicked() {
    _ui->_searchText->clear();
}

void SearchUICreator::searchTextChanged( QString query ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    bool oldVisible = _clearButton->isVisible();
    bool setVisible = ( query.length() > 0 );

    if ( oldVisible != setVisible ) {
        _clearButton->setVisible( setVisible ); //show clearButton if there is some text
        int w = ( setVisible ) ? 109 : 125;
        _ui->_searchText->setMaximumWidth( w );
    }

    SearchChangedEvent ev( query );
    Aggregator::instance()->sendEvent( &ev );
}

void SearchUICreator::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );

    if( !_backgroundImage.isNull() )  {
        QPixmap img = _backgroundImage.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::FastTransformation );
        painter.drawPixmap( 0, 0, img );
    }

    if( !_backgroundImageSearchTextBox.isNull() ) {
        for( int i = 0; i < 140; i++ )
            painter.drawPixmap( width() - i, 5, _backgroundImageSearchTextBox );
    }

    if( !_imageSearchBoxHead.isNull() )
        painter.drawPixmap( width() - 168, 5, _imageSearchBoxHead );

    //searchBoxCover drawing
    if( !_imageSearchBoxCover.isNull() &&
            !_ui->_searchText->hasFocus() &&
            _ui->_searchText->text().isEmpty() ||
            !_searchEnabledForCurrentPlaylist ) {
        painter.drawPixmap( width() - 125, 6, _imageSearchBoxCover );
    }

    if( !_hasMainUIRegionFocus ) {
        QPixmap overlayPicture( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_OVERLAY_IMAGE );
        painter.setBrush( QBrush( overlayPicture ) );
        painter.drawRect( -1, -1, width() + 1, height() + 1 );
    } else {
    }
}

void SearchUICreator::changeEvent( QEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SearchUICreator::enterEvent( QEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    emitMainUIRegionFocusedChangedEvent();
}

void SearchUICreator::emitMainUIRegionFocusedChangedEvent() {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::TOP );
    Aggregator::instance()->sendEvent( &e );
}

void SearchUICreator::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );

    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    if( _hasMainUIRegionFocus ) {
        _ui->_playlistName->setStyleSheet( _titleStyleSheetLight );

        if( _searchGloballyEnabled ) {
            _ui->_searchText->setEnabled( true );
            _ui->_searchText->setFocus();
        }
    } else {
        _ui->_playlistName->setStyleSheet( _titleStyleSheetDark );

        if( _searchGloballyEnabled ) {
            _ui->_searchText->clearFocus();
            _ui->_searchText->setEnabled( false );
        }
    }

    repaint();
}

void SearchUICreator::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( widgetType == CentralFocusChangedEvent::ABOUT_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::TV_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::NEWS_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::VIDEOPLAYER ||
            widgetType == CentralFocusChangedEvent::SETTINGS_PLAYLIST ) {
        _ui->_searchText->setVisible( false );
        _searchEnabledForCurrentPlaylist = false;
        _clearButton->hide();
    }

    if( widgetType == CentralFocusChangedEvent::COVERVIEW_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::LISTVIEW_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::GRIDVIEW_PLAYLIST ) {
        _ui->_searchText->setVisible( true );
        _searchEnabledForCurrentPlaylist = true;

        if( _ui->_searchText->text().length() > 0 )
            _clearButton->show();
    }

    update();
}

void SearchUICreator::enableSearchGlobally() {
    _searchGloballyEnabled = true;

    if( _hasMainUIRegionFocus && _searchEnabledForCurrentPlaylist )
        _ui->_searchText->setFocus();
    else
        _ui->_searchText->clearFocus();

    repaint();
}

void SearchUICreator::resolveStyleFactory() {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, Q_FUNC_INFO );
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
        SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, "Resolving succeded" );
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void SearchUICreator::setupStyle() {
    if( _styleFactory == NULL )
        return;

    setupTitleStyleLight();
    setupTitleStyleDark();
    setupSearchBoxStyle();
}

void SearchUICreator::setupTitleStyleLight() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "TopRegion_TitleText", &ok );

    if( ok ) {
        _titleStyleSheetLight = QString( "background-color:transparent;" );
        _titleStyleSheetLight += titleTextStyle->styleSheet();
        _ui->_playlistName->setStyleSheet( _titleStyleSheetLight );
        _ui->_playlistName->setAlignment( titleTextStyle->alignment() );
    }
}

void SearchUICreator::setupTitleStyleDark() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "TopRegion_TitleText_Dark", &ok );

    if( ok ) {
        _titleStyleSheetDark = QString( "background-color:transparent;" );
        _titleStyleSheetDark += titleTextStyle->styleSheet();
    }
}

void SearchUICreator::setupSearchBoxStyle() {
    bool ok;
    ITextStyle *searchBoxTextStyle = _styleFactory->getTextStyle( "SearchUI_SearchBoxText", &ok );

    if( ok ) {
        QString searchBoxStyle = QString( "background-color:transparent;" );
        searchBoxStyle += searchBoxTextStyle->styleSheet();
        _ui->_searchText->setStyleSheet( searchBoxStyle );
        _ui->_searchText->setAlignment( searchBoxTextStyle->alignment() );
    }
}


