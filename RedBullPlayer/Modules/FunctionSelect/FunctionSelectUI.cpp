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
#include "FunctionSelectUI.h"
#include "ui_FunctionSelectUI.h"

// QT includes
#include <QPainter>
#include <QBitmap>
#include <QDebug>

// Project Includes
#include "../../Widgets/TransparentImage.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Events;


FunctionSelectUI::FunctionSelectUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::FunctionSelectUI ) {
    _ui->setupUi( this );
    _iconCarrousel = new IconCarrousel( _ui->_mainWidget );
    connect( _iconCarrousel, SIGNAL( animationFinished() ), this, SLOT( animationFinishedSlot() ) );
    connect( _iconCarrousel, SIGNAL( iconClicked( QString ) ), this, SLOT( iconClickedSlot( QString ) ) );
    setNavigation();
    QImage img( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_CIRCLE );
    _ellipse = img.scaledToWidth( 700, Qt::SmoothTransformation );
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::BOTTOM;
}

FunctionSelectUI::~FunctionSelectUI() {
    delete _ui;
}

void FunctionSelectUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

QWidget *FunctionSelectUI::getMainWidget() {
    return _ui->_mainWidget;
}

void FunctionSelectUI::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.drawImage( _ui->_mainWidget->x(), 35, _ellipse );
}

void FunctionSelectUI::setNavigation() {
    TransparentImage *left = new TransparentImage( Constants::FUNCTION_SELECT_NAVIGATION_LEFT_ARROW_URL,
            Constants::ARROW_ALPHA, _ui->_mainWidget );
    left->setGeometry( 30, 15, 25, 35 );
    connect( left, SIGNAL( clicked() ), this, SLOT( onLeftNavigationClicked() ) );
    TransparentImage *right = new TransparentImage( Constants::FUNCTION_SELECT_NAVIGATION_RIGHT_ARROW_URL,
            Constants::ARROW_ALPHA, _ui->_mainWidget );
    right->setGeometry( 645, 15, 25, 35 );
    connect( right, SIGNAL( clicked() ), this, SLOT( onRightNavigationClicked() ) );
}

void FunctionSelectUI::onLeftNavigationClicked() {
    _iconCarrousel->scrollLeft();
}

void FunctionSelectUI::onRightNavigationClicked() {
    _iconCarrousel->scrollRight();
}

void FunctionSelectUI::animationFinishedSlot() {
}

void FunctionSelectUI::iconDoubleClickedSlot( QString /*title*/ ) {
}

void FunctionSelectUI::iconClickedSlot( QString title ) {
    emit functionChanged( title );
}

void FunctionSelectUI::selectItemInTheMiddle() {
    _iconCarrousel->selectIconInTheMiddle();
}

void FunctionSelectUI::addWidget( QString url, QString activeUrl, QString darkUrl, QString darkActiveUrl, QString title ) {
    _iconCarrousel->addWidget( url, activeUrl, darkUrl, darkActiveUrl, title );
}

void FunctionSelectUI::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    _iconCarrousel->changeFocusStatus( _hasMainUIRegionFocus );
    repaint();
}

bool FunctionSelectUI::hasMainFocus() {
    return _hasMainUIRegionFocus;
}

void FunctionSelectUI::changeInternetConnectionStatus( bool isConnected ) {
    _iconCarrousel->changeInternetConnectionStatus( isConnected );
}
