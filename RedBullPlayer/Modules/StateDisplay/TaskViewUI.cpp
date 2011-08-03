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
#include "TaskViewUI.h"
#include "ui_TaskViewUI.h"

#include <QHBoxLayout>
#include <QFontMetrics>

//Project includes
#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::TaskManager;

TaskViewUI::TaskViewUI( QWidget *parent ) :
    RedBullPlayer::Modules::StateDisplay::ITaskView( parent ),
    _ui( new Ui::TaskViewUI ) {
    _ui->setupUi( this );
    initCancelButton();
    initCircleProgessAnimation();
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; color: black;}" );
}

TaskViewUI::~TaskViewUI() {
    delete _ui;
}

void TaskViewUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void TaskViewUI::resizeEvent( QResizeEvent * ) {
    setNameLabelText();
}

void TaskViewUI::initTaskViewSize() {
    int taskViewWidth = ConfigManager::instance()->hoverBoxWidth() - 2 * ConfigManager::instance()->hoverBoxBorderWidth();
    int nameLabelWidth = taskViewWidth -
                         _ui->_nameLabel_circeProgress_Spacer->geometry().width() -
                         _ui->_progressCircleLabel->width() -
                         _ui->_progressDisplayLabel->width() -
                         _ui->_cancelButtonProgressDisplaySpacer->geometry().width() -
                         _ui->_circeProgress_progressDisplay_Spacer->geometry().width() -
                         _ui->_cancelButtonContainer->width();
    _ui->_nameLabel->setFixedWidth( nameLabelWidth );
}

void TaskViewUI::setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) {
    _ui->_nameLabel->setStyleSheet( textStyle->styleSheet() );
    _ui->_progressDisplayLabel->setStyleSheet( textStyle->styleSheet() );
}

void TaskViewUI::setProgressDisplayText( QString text ) {
    _ui->_progressDisplayLabel->setText( text );
}

void TaskViewUI::initCancelButton() {
    QHBoxLayout *layout = qobject_cast<QHBoxLayout*>( _ui->_cancelButtonContainer->layout() );

    if( layout == NULL )
        return;

    _cancelButton = new CommandButton( this, Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CANCEL_BUTTON );
    _cancelButton->setMouseOverImage( Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CANCEL_BUTTON_PRESSED );
    connect( _cancelButton, SIGNAL( clicked() ), this, SLOT( cancelClickedSlot() ) );
    layout->addWidget( _cancelButton );
}

void TaskViewUI::initCircleProgessAnimation() {
    _circleAnimation = new QMovie( Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CIRCLE_PROGRESS_ANIMATION );

    if( _circleAnimation->isValid() ) {
        _ui->_progressCircleLabel->setMovie( _circleAnimation );
    }
}

void TaskViewUI::startCircleAnimation() {
    if( _circleAnimation == NULL || !_circleAnimation->isValid() )
        return;

    _circleAnimation->start();
}

void TaskViewUI::stopCircleAnimation() {
    //stops the animation and jumps to frame e 0 so nothing is displayed
    if( _circleAnimation == NULL || !_circleAnimation->isValid() )
        return;

    _circleAnimation->jumpToFrame( 0 );
    _circleAnimation->stop();
}

void TaskViewUI::setTaskInfo( TaskInfo taskInfo ) {
    _taskInfo = taskInfo;
    setNameLabelText();
}

void TaskViewUI::setText( QString text ) {
    //this is realy bad design... should talk to vlad about "setNameLabelText" and similar functions..
    _taskInfo.second = text;
    setNameLabelText();
}

void TaskViewUI::setNameLabelText() {
    if( _taskInfo.second.isEmpty() )
        return;

    QString labelText = _taskInfo.second;
    QFontMetrics fontMetrics = QFontMetrics( _ui->_nameLabel->font() );
    labelText = fontMetrics.elidedText( labelText, Qt::ElideRight, _ui->_nameLabel->width() );
    _ui->_nameLabel->setText( labelText );
    setToolTip( _taskInfo.second );
}

void TaskViewUI::cancelClickedSlot() {
    emit cancelClicked( _taskInfo.first );
}

void TaskViewUI::activate() {
    startCircleAnimation();
}

void TaskViewUI::deactivate() {
    stopCircleAnimation();
    setProgressDisplayText( "0%" );
}

void TaskViewUI::showCancelButton() {
    _cancelButton->setVisible( true );
    _ui->_cancelButtonContainer->setVisible( true );
}

void TaskViewUI::hideCancelButton() {
    _cancelButton->setVisible( false );
    _ui->_nameLabel->setFixedWidth( _ui->_nameLabel->width() + _ui->_cancelButtonContainer->width() );
    _ui->_cancelButtonContainer->setFixedWidth( 0 );
}
