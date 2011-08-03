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
#include "HoverBoxUI.h"
#include "ui_HoverBoxUI.h"

#include <QPainter>
#include <QScrollBar>

//Project includes
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::StateDisplay;

HoverBoxUI::HoverBoxUI( QString cancelAllImageUrl, QString cancelAllMouseOverImageUrl, QWidget *parent ) :
    RedBullPlayer::Modules::StateDisplay::IStateView( parent ),
    _ui( new Ui::HoverBoxUI ) {
    _ui->setupUi( this );
    setAttribute( Qt::WA_TranslucentBackground, true );
    _cancelAllImageUrl = cancelAllImageUrl;
    _cancelAllMouseOverImageUrl = cancelAllMouseOverImageUrl;
    setWindowAttributes();
    initHoverBoxSize();
    initLayout();
    initCancelAllButton();
    _id = RedBullPlayer::Tools::UuidCreator::create();
    styleScrollBar();
}

HoverBoxUI::~HoverBoxUI() {
    delete _ui;
}

void HoverBoxUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void HoverBoxUI::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 1, height() + 1 );
}

void HoverBoxUI::setWindowAttributes() {
    setWindowFlags( Qt::Tool | Qt::FramelessWindowHint );
    setAttribute( Qt::WA_QuitOnClose );
    setAttribute( Qt::WA_MacAlwaysShowToolWindow, false );
}

void HoverBoxUI::initHoverBoxSize() {
    int borderWidth = ConfigManager::instance()->hoverBoxBorderWidth();
    int contentWidth = ConfigManager::instance()->hoverBoxWidth() - 2 * borderWidth;
    _taskViewContentAreaMaximumHeight = ConfigManager::instance()->hoverBoxContentRegionHeight();
    _ui->topVerticalLayout->setMargin( borderWidth );
    _ui->_contentWrapper->setMinimumWidth( contentWidth );
    _ui->_contentWrapper->setMaximumWidth( contentWidth );
    _ui->_middleScrollArea->setWidget( _ui->_taskViewContent );
    _ui->_taskViewContent->setFixedWidth( contentWidth );
    _ui->_bottomContainer->setFixedWidth( contentWidth );
    _taskViewContentAreaWidthScrollBarVisible = contentWidth - 18;
    _taskViewContentAreaWidthScrollBarNotVisible = contentWidth;
}

void HoverBoxUI::initLayout() {
    _taskViewContentLayout = qobject_cast<QBoxLayout*>( _ui->_taskViewContent->layout() );
}

void HoverBoxUI::initCancelAllButton() {
    QHBoxLayout *layout = qobject_cast<QHBoxLayout*>( _ui->_cancelAllContainer->layout() );

    if( layout == NULL )
        return;

    _cancelAllButton = new CommandButton( this, _cancelAllImageUrl );
    _cancelAllButton->setMouseOverImage( _cancelAllMouseOverImageUrl );
    connect( _cancelAllButton, SIGNAL( clicked() ), this, SIGNAL( cancelAllClicked() ) );
    layout->insertWidget( 1, _cancelAllButton );
}

void HoverBoxUI::setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) {
    _ui->_titleLabel->setStyleSheet( textStyle->styleSheet() );
}

void HoverBoxUI::setTitle( QString title ) {
    _ui->_titleLabel->setText( title );
}

QUuid HoverBoxUI::guid() const {
    return _id;
}

void HoverBoxUI::setTaskViewContentHeight( int newHeight ) {
    _ui->_taskViewContent->setFixedHeight( newHeight );

    if( newHeight <= _taskViewContentAreaMaximumHeight )
        _ui->_middleScrollArea->setFixedHeight( newHeight );
}

void HoverBoxUI::setTaskViewContentWidth( int newHeight ) {
    if( newHeight <= _taskViewContentAreaMaximumHeight )
        _ui->_taskViewContent->setFixedWidth( _taskViewContentAreaWidthScrollBarNotVisible );
    else
        _ui->_taskViewContent->setFixedWidth( _taskViewContentAreaWidthScrollBarVisible );
}

void HoverBoxUI::append( RedBullPlayer::Modules::StateDisplay::ITaskView *taskView ) {
    if ( _taskItems.contains( taskView->taskInfo().first ) )
        return;

    if( _taskViewContentLayout == NULL )
        return;

    int newContentContainerHeight = _ui->_taskViewContent->height() +  taskView->height();
    _taskItems.insert( taskView->taskInfo().first, taskView );
    _taskViewContentLayout->addWidget( taskView );
    setTaskViewContentHeight( newContentContainerHeight );
    setTaskViewContentWidth( newContentContainerHeight );
}

void HoverBoxUI::remove( QUuid taskId ) {
    if ( _taskItems.contains( taskId ) ) {
        RedBullPlayer::Modules::StateDisplay::ITaskView* taskViewToRemove = _taskItems.value( taskId );

        if( _taskViewContentLayout == NULL )
            return;

        int newContentContainerHeight = _ui->_taskViewContent->height() -  taskViewToRemove->height();
        _taskViewContentLayout->removeWidget( taskViewToRemove );
        setTaskViewContentHeight( newContentContainerHeight );
        setTaskViewContentWidth( newContentContainerHeight );
        _taskItems.remove( taskId );
        taskViewToRemove->deactivate();
        delete taskViewToRemove;
    }
}

void HoverBoxUI::activate( QUuid taskId ) {
    if ( _taskItems.contains( taskId ) ) {
        RedBullPlayer::Modules::StateDisplay::ITaskView* taskViewToUpdate = _taskItems.value( taskId, NULL );
        taskViewToUpdate->activate();
    }
}

void HoverBoxUI::updateText( QUuid taskId, QString text ) {
    if ( _taskItems.contains( taskId ) ) {
        ITaskView* taskView = _taskItems.value( taskId, 0 );

        if ( taskView != 0 )
            taskView->setText( text );
    }
}

void HoverBoxUI::updateProgressText( QUuid taskId, QString text ) {
    if ( _taskItems.contains( taskId ) ) {
        ITaskView* taskView = _taskItems.value( taskId, 0 );

        if ( taskView != 0 )
            taskView->setProgressDisplayText( text );
    }
}

void HoverBoxUI::showCancelAll() {
    _cancelAllButton->setVisible( true );
}

void HoverBoxUI::hideCancelAll() {
    _cancelAllButton->setVisible( false );
}

void HoverBoxUI::enterEvent( QEvent * ) {
    emit mouseEnter();
}

void HoverBoxUI::leaveEvent( QEvent * ) {
    emit mouseLeave();
}

void HoverBoxUI::show( int x, int y ) {
    if( !isVisible() ) {
        QWidget::move( x, y );
        QWidget::show();
    } else {
        QWidget::move( x, y );
    }
}

void HoverBoxUI::hide() {
    if( isVisible() )
        QWidget::hide();
}

void HoverBoxUI::styleScrollBar() {
    QString scrollBarStyleSheet =  "QScrollBar:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_background);"
                                   "width: 18px;"
                                   "margin: 16px 0 16px 0;"
                                   "}"
                                   "QScrollBar::handle:vertical {"
                                   "background-image: url(:/scroll_slider);"
                                   "min-height: 20px;"
                                   "margin-left:2px;"
                                   "}"
                                   "QScrollBar::add-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_down);"
                                   "height:16px;"
                                   "subcontrol-position: bottom;"
                                   "subcontrol-origin: margin;"
                                   "margin-left:1px;"
                                   "}"
                                   "QScrollBar::sub-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_up);"
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
                                   "}";
    setStyleSheet( scrollBarStyleSheet );
    _ui->_middleScrollArea->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
}
