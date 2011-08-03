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
#include "BottomUICreator.h"
#include "ui_BottomUICreator.h"

// Qt Includes
#include <QPainter>
#include <QDebug>
#include "math.h"

// Project Includes
#include "../../Container/Constants.h"
#include "../../Widgets/DraggableLabel.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;

BottomUICreator::BottomUICreator( RedBullPlayer::PlayerShell::Shell* shell, QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::BottomUICreator ),
    _shell( shell ) {
    _ui->setupUi( this );
    initResizeWidget();
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::BOTTOM;
    _upperGradient = QPixmap( Constants::BOTTOM_UI_IMAGE_DIR + Constants::BOTTOM_UI_LOWERGRADIENT_URL );
    _backgroundPattern = QPixmap( Constants::BOTTOM_UI_IMAGE_DIR + Constants::BOTTOM_UI_BG_URL );
    _grayLineWidthToTotalWidthRatio = 0.812;
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "FunctionSelectRegion", _ui->_functionSelectRegionFrame ) );

    //NewsTicker On OFF Code
    if( ConfigManager::instance()->newsTickerOn() ) {
        _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "NewsTickerRegion", _ui->_newsTickerRegionFrame ) );
    } else {
        _ui->_newsTickerRegionFrame->setVisible( false );
    }
}

BottomUICreator::~BottomUICreator() {
    delete _ui;
}

void BottomUICreator::initResizeWidget() {
    DraggableLabel *resizeLabel = new DraggableLabel( _ui->_resizeWidgetContainer );
    QPixmap pixmap( Constants::BOTTOM_UI_IMAGE_DIR + Constants::BOTTOM_UI_RESIZE_URL );
    resizeLabel->setPixmap( pixmap );
    QBoxLayout *resizeLayout = qobject_cast<QBoxLayout *>( _ui->_resizeWidgetContainer->layout() );

    if( resizeLayout != NULL ) {
        resizeLayout->insertWidget( 1, resizeLabel );
    }

    connect( resizeLabel, SIGNAL( dragging( QPoint ) ), this, SLOT( draggingSlot( QPoint ) ) );
    connect( resizeLabel, SIGNAL( clicked() ), this, SIGNAL( resizeWidgetClicked() ) );
}

void BottomUICreator::draggingSlot( QPoint distance ) {
    emit dragging( distance );
}

void BottomUICreator::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );

    if( !_backgroundPattern.isNull() ) {
        int top = width() / _backgroundPattern.width() + 1;

        for( int i = 0; i < top; i++ ) {
            painter.drawPixmap( i * _backgroundPattern.width(), 0, _backgroundPattern );
        }
    }

    if( !_upperGradient.isNull() ) {
        int widthLine = (int) round( width() * _grayLineWidthToTotalWidthRatio );
        QPixmap line = _upperGradient.scaledToWidth( width(),
                       Qt::FastTransformation );
        painter.drawPixmap( 0, 0, widthLine, line.height(), line, 0, 0, widthLine, line.height() );
    }

    //overlay drawing if case
    if( !_hasMainUIRegionFocus ) {
        QPixmap overlayPicture( Constants::BOTTOM_UI_IMAGE_DIR + Constants::BOTTOM_UI_OVERLAY_IMAGE );
        painter.setBrush( QBrush( overlayPicture ) );
        painter.drawRect( -1, -1, width() + 1, height() + 1 );
    }
}

void BottomUICreator::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void BottomUICreator::enterEvent( QEvent */*e*/ ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_BOTTOMUI, Q_FUNC_INFO );
    emitMainUIRegionFocusedChangedEvent();
}

void BottomUICreator::emitMainUIRegionFocusedChangedEvent() {
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::BOTTOM );
    Aggregator::instance()->sendEvent( &e );
}

void BottomUICreator::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    repaint();
}




