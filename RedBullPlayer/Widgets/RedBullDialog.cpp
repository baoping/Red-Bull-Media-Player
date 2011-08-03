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
#include "RedBullDialog.h"
#include "ui_RedBullDialog.h"

#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QLabel>
#include <QBitmap>
#include <QDesktopWidget>

#include "CommandButton.h"

using namespace RedBullPlayer::Widgets;

RedBullDialog::RedBullDialog( QString styleSheet,
                              RedBullPlayer::Widgets::RedBullDialog::Buttons buttons,
                              QWidget *parent ) :
    QDialog( parent,  Qt::FramelessWindowHint | Qt::Dialog ),
    ui( new Ui::RedBullDialog ) {
    setAttribute( Qt::WA_TranslucentBackground, true );
    bootstrap( styleSheet, buttons );
    _backgroundImage = QPixmap( ":/dialog_background" );
}

RedBullDialog::RedBullDialog( QString text,
                              QString styleSheet,
                              RedBullPlayer::Widgets::RedBullDialog::Buttons buttons,
                              QWidget *parent ) :
    QDialog( parent, Qt::FramelessWindowHint | Qt::Dialog ),
    ui( new Ui::RedBullDialog ) {
    setAttribute( Qt::WA_TranslucentBackground, true );
    bootstrap( styleSheet, buttons );
    addLabel( text );
    _backgroundImage = QPixmap( ":/dialog_background" );
}

void RedBullDialog::bootstrap( QString styleSheet,
                               RedBullPlayer::Widgets::RedBullDialog::Buttons buttons ) {
    _styleSheet = ( styleSheet + " background:transparent; border:none" );
    setButtons( buttons );
    ui->setupUi( this );
    setupButtons();
}

RedBullDialog::~RedBullDialog() {
    _checkBoxes.clear();
    delete ui;
}

void RedBullDialog::setupButtons() {
    if ( _buttons.testFlag( Ok ) ) {
        CommandButton *btn = new CommandButton( this, ":/dialog_button_ok" );
        btn->setMouseOverImage( ":/dialog_button_ok_pressed" );
        btn->setFlat( true );
        btn->setFocusPolicy( Qt::NoFocus );
        connect( btn, SIGNAL( clicked() ),
                 this, SLOT( accept() ) );
        btn->setFixedSize( 85, 27 );
        ui->_buttonsLayout->insertWidget( 1, btn );
    }

    // Request Cancel Images
    //    if ( _buttons.testFlag( Cancel ) ) {
    //	CommandButton *btn = new CommandButton(this, ":/dialog_button_ok");
    //	btn->setMouseOverImage( ":/dialog_button_ok_pressed" );
    //	btn->setFlat( true );
    //	btn->setFocusPolicy( Qt::NoFocus );
    //	connect(btn,SIGNAL(clicked()),
    //		this,SLOT(reject()));
    //	btn->setFixedSize( 85, 27 );
    //	ui->_buttonLayout->addWidget( btn );
    //    }
}

void RedBullDialog::reject() {
    QDialog::reject();
}

void RedBullDialog::accept() {
    QDialog::accept();
}

void RedBullDialog::setButtons( RedBullDialog::Buttons buttons ) {
    _buttons = buttons;
}

RedBullDialog::Buttons RedBullDialog::buttons() const {
    return _buttons;
}

void RedBullDialog::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 2, height() + 2 );
}


void RedBullDialog::changeEvent( QEvent *e ) {
    QDialog::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void RedBullDialog::addLabel( QString text ) {
    QLabel* label = new QLabel( text, this );
    label->setWordWrap( true );

    if ( !_styleSheet.isEmpty() ) {
        label->setStyleSheet( _styleSheet );
    }

    ui->bodyLayout->addWidget( label );
}

void RedBullDialog::addWidget( QWidget* widget ) {
    widget->setParent( this );
    ui->bodyLayout->addWidget( widget );
}

void RedBullDialog::addCheckBox( QString referenceName, QString text, bool defaultValue ) {
    QCheckBox* checkBox = new QCheckBox( text, this );

    if ( !_styleSheet.isEmpty() ) {
        checkBox->setStyleSheet( _styleSheet );
    }

    if ( !referenceName.isEmpty() ) {
        _checkBoxes.insert( referenceName, checkBox );
    }

    checkBox->setChecked( defaultValue );
    ui->bodyLayout->addWidget( checkBox );
}

bool RedBullDialog::getChecked( QString referenceName ) {
    if ( _checkBoxes.contains( referenceName ) ) {
        return _checkBoxes.value( referenceName )->isChecked();
    }

    return false;
}

void RedBullDialog::setChecked( QString referenceName, bool value ) {
    if ( _checkBoxes.contains( referenceName ) ) {
        _checkBoxes.value( referenceName )->setChecked( value );
    }
}
