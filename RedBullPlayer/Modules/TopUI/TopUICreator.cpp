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
#include "TopUICreator.h"
#include "ui_TopUICreator.h"

// Qt Includes
#include <QDebug>
#include <QPainter>
#include <QMessageBox>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Widgets/ClickableLabel.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;

TopUICreator::TopUICreator( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::TopUICreator ) {
    _ui->setupUi( this );
    configureLogo();
    configureExitButton();
    configureStatusDisplay( parent );
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::TOP;
    _topLineImage = QImage( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_TOPLINE_IMAGE );
}

TopUICreator::~TopUICreator() {
    delete _ui;
}

void TopUICreator::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );
    QImage origImg( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_BG_FILENAME );

    if( !origImg.isNull() ) {
        int top = width() / origImg.width() + 1;

        for( int i = 0; i < top; i++ ) {
            painter.drawImage( i * origImg.width(), 0, origImg );
        }
    }

    if( !_hasMainUIRegionFocus ) {
        QPixmap overlayPicture( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_OVERLAY_IMAGE );
        painter.setBrush( QBrush( overlayPicture ) );
        painter.drawRect( -1, -1, width() + 1, height() + 1 );
    }
}

void TopUICreator::configureStatusDisplay( QWidget *parent ) {
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_statusIconsContainer->layout() );
    _stateController = new StateIconController( parent, layout, this );
}

void TopUICreator::configureLogo() {
    //used Only for Drag Drop Operations
    _logoLabel = new DraggableLabel( this );
    _logoLabel->setStyleSheet( "background-color:transparent" );
    QPixmap logoPixmap( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_LOGO_FILENAME );
    QBoxLayout *baseLayout = qobject_cast<QBoxLayout*>(  _ui->_logoContainer->layout() );

    if( baseLayout != NULL ) {
        baseLayout->insertWidget( 0, _logoLabel );
    } else {
        qWarning() << tr( "TopUICreator: Cannot display logo because TopUICreator layout cannot be cast to QBoxLayout" );
    }

    connect( _logoLabel, SIGNAL( dragging( QPoint ) ), this, SLOT( draggingSlot( QPoint ) ) );
    connect( _logoLabel, SIGNAL( doubleClicked() ), this, SLOT( logoDoubleClicked() ) );
}

void TopUICreator::configureExitButton() {
    ClickableLabel *exitLabel = new ClickableLabel( this );
    exitLabel->setStyleSheet( "background-color:transparent" );
    QPixmap exitPixmap( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_EXIT_FILENAME );
    exitLabel->setPixmap( exitPixmap );
    connect( exitLabel, SIGNAL( clicked() ), this, SLOT( exitClickedSlot() ) );
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_exitContainer->layout() );

    if( layout != NULL ) {
        layout->insertWidget( 1, exitLabel );
    } else {
        qWarning() << tr( "Cannot display exit button because exitContainer layout cannot be cast to QBoxLayout" );
    }
}



void TopUICreator::exitClickedSlot() {
    emit exitClicked();
}

void TopUICreator::draggingSlot( QPoint distance ) {
    emit dragging( distance );
}

void TopUICreator::mousePressEvent ( QMouseEvent * ev ) {
    _logoLabel->invokeMouseEvent( DraggableLabel::PRESS, ev );
    setCursor( QCursor( Qt::SizeAllCursor ) );
}

void TopUICreator::mouseMoveEvent ( QMouseEvent * ev ) {
    _logoLabel->invokeMouseEvent( DraggableLabel::MOVE, ev );
}

void TopUICreator::mouseReleaseEvent ( QMouseEvent *ev ) {
    _logoLabel->invokeMouseEvent( DraggableLabel::RELEASE, ev );
    setCursor( QCursor( Qt::ArrowCursor ) );
}

void TopUICreator::mouseDoubleClickEvent ( QMouseEvent * /*ev*/ ) {
    emit doubleClicked();
}

void TopUICreator::logoDoubleClicked() {
    emit doubleClicked();
}

void TopUICreator::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void TopUICreator::enterEvent( QEvent */*e*/ ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TOPUI, Q_FUNC_INFO );
    emitMainUIRegionFocusedChangedEvent();
}

void TopUICreator::emitMainUIRegionFocusedChangedEvent() {
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::TOP );
    Aggregator::instance()->sendEvent( &e );
}

void TopUICreator::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    repaint();
}

void TopUICreator::setLogoEnabled( bool enabled ) {
    QPixmap logoPixmap;

    if( enabled ) {
        logoPixmap = QPixmap( Constants::TOP_UI_IMAGE_DIR + Constants::TOP_UI_LOGO_FILENAME );
    } else {
        logoPixmap = QPixmap( Constants::TOP_UI_IMAGE_DIR + "offline" + Constants::TOP_UI_LOGO_FILENAME );
    }

    //_logoLabel->setPixmap(logoPixmap);
}

void TopUICreator::initStateDisplay( IStyleFactory *styleFactory, IMediaLibrary *mediaLibrary ) {
    _stateController->init( styleFactory, mediaLibrary );
}


