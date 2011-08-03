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
#include "DeleteBox.h"
#include "ui_DeleteBox.h"

// QT includes
#include <QDebug>
#include <QPainter>
#include <QBoxLayout>
#include <QBitmap>
#include <QString>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Widgets/Button.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::StyleFactory;

DeleteBox::DeleteBox( QString text, ITextStyle *textStyle, QWidget *parent ) :
    QDialog( parent, Qt::Window |  Qt::Dialog | Qt::FramelessWindowHint ),
    _ui( new Ui::DeleteBox ) {
    setAttribute( Qt::WA_TranslucentBackground, true );
    _ui->setupUi( this );
    setupTextStyle( textStyle );
    _ui->_text->setText( text );
    _backgroundImage = QPixmap( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_DELETE_BG_URL );

    if( _ui->_containerWidget != NULL ) {
        QBoxLayout *layout = qobject_cast<QBoxLayout *>( _ui->_containerWidget->layout() );

        if( layout != NULL ) {
            QLabel *img = new QLabel( this );
            img->setPixmap( QPixmap( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_DELETE_BIN_ICON_URL ) );
            layout->insertWidget( 0, img );
            Button *yes = new Button( this,
                                      Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_DELETE_YES_ICON_URL,
                                      Constants::PLAYLISTUI_IMAGE_DIR + "active" + Constants::PLAYLISTUI_DELETE_YES_ICON_URL,
                                      Constants::PLAYLISTUI_IMAGE_DIR + "active" + Constants::PLAYLISTUI_DELETE_YES_ICON_URL );
            layout->insertWidget( 2, yes );
            connect( yes, SIGNAL( clicked() ), this, SLOT( yesClicked() ) );
            Button *no = new Button( this,
                                     Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_DELETE_NO_ICON_URL,
                                     Constants::PLAYLISTUI_IMAGE_DIR + "active" + Constants::PLAYLISTUI_DELETE_NO_ICON_URL,
                                     Constants::PLAYLISTUI_IMAGE_DIR + "active" + Constants::PLAYLISTUI_DELETE_NO_ICON_URL );
            layout->insertWidget( 4, no );
            connect( no, SIGNAL( clicked() ), this, SLOT( noClicked() ) );
        }
    }
}

void DeleteBox::setupTextStyle( ITextStyle *textStyle ) {
    if( textStyle == NULL )
        return;

    QString textStyleSheet = QString( "background-color:transparent;" );
    textStyleSheet += textStyle->styleSheet();
    _ui->_text->setStyleSheet( textStyleSheet );
    _ui->_text->setAlignment( textStyle->alignment() );
}

DeleteBox::~DeleteBox() {
    delete _ui;
}

void DeleteBox::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 2, height() + 2 );
}

void DeleteBox::yesClicked() {
    done( 1 );
}

void DeleteBox::noClicked() {
    done( 0 );
}

void DeleteBox::changeEvent( QEvent *e ) {
    QDialog::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}
