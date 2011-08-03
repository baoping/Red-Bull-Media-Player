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
#include "UpdateProgressWidget.h"
#include "ui_UpdateProgressWidget.h"

#include <QPainter>
#include <QBitmap>
#include <QDesktopWidget>


#include "CommandButton.h"


using namespace RedBullPlayer::Widgets;

UpdateProgressWidget::UpdateProgressWidget( QWidget *parent ) :
    QWidget( parent, Qt::Window | Qt::FramelessWindowHint | Qt::Dialog ),
    m_ui( new Ui::UpdateProgressWidget ) {
    setAttribute( Qt::WA_TranslucentBackground, true );
    m_ui->setupUi( this );
    m_ui->_progress->setValue( 0 );
    setWindowFlags( Qt::FramelessWindowHint );
    _backgroundImage = QPixmap( ":/dialog_background" );
    _logoImage = QPixmap( ":/dialog_icon" );

    if( !_logoImage.isNull() )
        m_ui->_logoLabel->setPixmap( _logoImage );

    QRect scr = QApplication::desktop()->screenGeometry( QApplication::desktop()->screenNumber( parentWidget() ) );
    move( scr.center() - rect().center() );
    QPixmap line = QPixmap( ":/update_process_streifen" );
    line = line.scaledToWidth( width(), Qt::FastTransformation );
    m_ui->_linieLabel->setPixmap( line );
}

UpdateProgressWidget::~UpdateProgressWidget() {
    delete m_ui;
}

void UpdateProgressWidget::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void UpdateProgressWidget::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 2, height() + 2 );
}

void UpdateProgressWidget::doStep( QString task ) {
    m_ui->_currentTaskName->setText( task );
    m_ui->_progress->setValue( m_ui->_progress->value() + 1 );
}

void UpdateProgressWidget::setSteps( int count ) {
    m_ui->_progress->setValue( 0 );
    m_ui->_progress->setMaximum( count );
}

void UpdateProgressWidget::setHeader( QString header ) {
    m_ui->_headerLabel->setText( header );
}

void UpdateProgressWidget::showOkButton() {
    CommandButton *btn = new CommandButton( this, ":/dialog_button_ok" );
    btn->setMouseOverImage( ":/dialog_button_ok_pressed" );
    btn->setFlat( true );
    btn->setFocusPolicy( Qt::NoFocus );
    connect( btn, SIGNAL( clicked() ),
             this, SIGNAL( okClicked() ) );
    btn->setFixedSize( 85, 27 );
    m_ui->_buttonsContainer->insertWidget( 0, btn );
}

void UpdateProgressWidget::showCancelButton() {
    CommandButton *btn = new CommandButton( this, ":/update_process_cancel" );
    btn->setMouseOverImage( ":/update_process_cancel_pressed" );
    btn->setFlat( true );
    btn->setFocusPolicy( Qt::NoFocus );
    connect( btn, SIGNAL( clicked() ),
             this, SIGNAL( cancelClicked() ) );
    btn->setFixedSize( 85, 27 );
    m_ui->_buttonsContainer->insertWidget( 0, btn );
}

