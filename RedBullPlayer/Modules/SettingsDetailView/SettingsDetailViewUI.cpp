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
#include "SettingsDetailViewUI.h"
#include "ui_SettingsDetailViewUI.h"

#include <QPainter>
#include <QBitmap>
#include <QList>
#include <QScrollBar>
#include <QTimer>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "SettingsViewFactory.h"
#include "SettingsPresenter.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;

SettingsDetailViewUI::SettingsDetailViewUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::SettingsDetailViewUI ) {
    _ui->setupUi( this );
    initExitButton();
    _moduleRegionWidget = parent;
    _backgroundImage = QPixmap( Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_GENERAL_BACKGROUND );
    _forceToShowNextTime = false;
    _hasCentralFocus = false;
    _hasMainFocus = false;
    _viewFactory = new SettingsViewFactory( this, this );
    _settingsPresenter = new SettingsPresenter( this, _viewFactory );
    connect( _settingsPresenter, SIGNAL( userSettingChanged() ), this, SIGNAL( userSettingChanged() ) );
    styleScrollBar();
    _widgetType = CentralFocusChangedEvent::SETTINGS_DETAIL;
    _mainUIRegion = MainUIRegionFocusChangedEvent::CENTRAL;
}

SettingsDetailViewUI::~SettingsDetailViewUI() {
    delete _ui;
}

void SettingsDetailViewUI::initExitButton() {
    _exitButton = new Button( this, Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_EXIT_BUTTON,
                              Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_EXIT_BUTTON_PRESSED,
                              Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_EXIT_BUTTON );
    connect( _exitButton, SIGNAL( clicked() ), this, SLOT( exitClicked() ) );
    _exitButton->setClickDownDistance( 0 );
    _ui->_exitButtonLayout->insertWidget( 0, _exitButton );
}

void SettingsDetailViewUI::enterEvent( QEvent *e ) {
    if( !_hasMainFocus )
        emitMainRegionFocusChanged();
}

void SettingsDetailViewUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SettingsDetailViewUI::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing );

    if( !_backgroundImage.isNull() ) {
        QPixmap scaledBackground = _backgroundImage.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        painter.drawPixmap( 0, 0, width(), height(), scaledBackground );
    }
}


void SettingsDetailViewUI::exitClicked() {
    _forceToShowNextTime = false;
    _ui->_titleLabel->setText( "" );
    _settingsPresenter->saveDataOnClose();
    clearContentArea();
    setVisibillity( false );
    emitCentralFocusChanged( false );
}

void SettingsDetailViewUI::handlePlaylistItemSelected( IMedia *mediaItem ) {
    _settingsPresenter->saveDataOnClose();
    clearContentArea();
    _forceToShowNextTime = true;
    _ui->_titleLabel->setText( mediaItem->title() );
    _backgroundImage = QPixmap( mediaItem->artworkUrl() );
    update();
    addSectionViews( _settingsPresenter->bootstrapViews( mediaItem->description() ) );
    setVisibillity( true );
}

void SettingsDetailViewUI::clearContentArea() {
    QLayoutItem *child;

    while ( ( child = _ui->_sectionsContainer->layout()->takeAt( 0 ) ) != 0 ) {
        child->widget()->deleteLater();
        delete child;
    }

    update();
}

void SettingsDetailViewUI::addSectionViews( QList<ISettingsSectionView *> sections ) {
    QBoxLayout *layout = qobject_cast<QBoxLayout *>( _ui->_sectionsContainer->layout() );

    if( layout != NULL )
        for( int i = 0; i < sections.count() ; i++ )
            layout->insertWidget( i, sections.at( i ) );
}

void SettingsDetailViewUI::handleCentralUiRegionResized() {
    refreshGeometry();
}

void SettingsDetailViewUI::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( _widgetType == widgetType )
        _hasCentralFocus = true;
    else
        _hasCentralFocus = false;
}

void SettingsDetailViewUI::handleMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent::UIRegion mainUiRegion ) {
    if( _mainUIRegion == mainUiRegion )
        _hasMainFocus = true;
    else
        _hasMainFocus = false;
}

void SettingsDetailViewUI::handleFunctionSelected( QString functionTitle ) {
    if( functionTitle == Constants::FUNCTION_SELECT_SETTINGS_TITLE && _forceToShowNextTime ) {
        setVisibillity( true );
        QTimer::singleShot( 1000, this, SLOT( refreshGeometry() ) ); //Hack because of false Event Handlig order so size can be corect
    } else {
        setVisibillity( false );
    }
}

void SettingsDetailViewUI::handleKeyPressed( int key ) {
    if( !_hasCentralFocus || !_hasMainFocus )
        return;

    if( key == Qt::Key_Escape )
        exitClicked();

    if( key == Qt::Key_Right )
        emitPlaylistNextItemSelected();

    if( key == Qt::Key_Left )
        emitPlaylistPreviousItemSelected();
}

void SettingsDetailViewUI::setVisibillity( bool visible ) {
    if( visible ) {
        _moduleRegionWidget->show();
        _moduleRegionWidget->setFocus();
        emitCentralFocusChanged( true );
    } else {
        _moduleRegionWidget->hide();
    }
}

void SettingsDetailViewUI::refreshGeometry() {
    if( _playlistRegion != NULL ) {
        _moduleRegionWidget->move( _playlistRegion->x() + _playlistRegion->width() / 4,
                                   _playlistRegion->y() + Constants::SETTINGS_DETAIL_VIEW_MARGIN_TOP );
        _moduleRegionWidget->resize( _playlistRegion->width() / 2,
                                     _playlistRegion->height()
                                     - Constants::SETTINGS_DETAIL_VIEW_MARGIN_BOTTOM
                                     - Constants::SETTINGS_DETAIL_VIEW_MARGIN_TOP );
    }
}

void SettingsDetailViewUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    setupTitleStyle();
}

void SettingsDetailViewUI::setupTitleStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "SettingsBox_HeaderText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color:transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_titleLabel->setStyleSheet( titleStyle );
        _ui->_titleLabel->setAlignment( titleTextStyle->alignment() );
    }
}

void SettingsDetailViewUI::styleScrollBar() {
    QString scrollBarStyleSheet =  "QScrollBar:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_background);"
                                   "background-repeat:repeat-y;"
                                   "width: 18px;"
                                   "margin: 16px 0 16px 0;"
                                   "}"
                                   "QScrollBar::handle:vertical {"
                                   "background-image: url(:/slider);"
                                   "min-height: 20px;"
                                   "margin-left:2px;"
                                   "}"
                                   "QScrollBar::add-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/down);"
                                   "height:16px;"
                                   "subcontrol-position: bottom;"
                                   "subcontrol-origin: margin;"
                                   "margin-left:1px;"
                                   "}"
                                   "QScrollBar::sub-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/up);"
                                   "height: 16px;"
                                   "subcontrol-position: top;"
                                   "subcontrol-origin: margin;"
                                   "margin-left:1px;"
                                   "}"
                                   "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                   "border: 0px;"
                                   "width: 0px;"
                                   "height: 0px;"
                                   "background: transparent;"
                                   "}"
                                   "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                   "background: none;"
                                   "}" ;
    setStyleSheet( scrollBarStyleSheet );
    _ui->_contentScrollArea->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
}

void SettingsDetailViewUI::setPlaylistRegionWidget( QWidget *playlistRegion ) {
    _playlistRegion = playlistRegion;
}

void SettingsDetailViewUI::setStyleFactory( IStyleFactory *styleFactory ) {
    _styleFactory = styleFactory;
    _viewFactory->setStyleFactory( styleFactory );
}

void SettingsDetailViewUI::setMediaLibrary( IMediaLibrary *mediaLibrary ) {
    _settingsPresenter->setMediaLibrary( mediaLibrary );
}
