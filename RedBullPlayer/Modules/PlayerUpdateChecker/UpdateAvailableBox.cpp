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
#include "UpdateAvailableBox.h"
#include "ui_UpdateAvailableBox.h"

// QT includes
#include <QDebug>
#include <QPainter>
#include <QBoxLayout>
#include <QBitmap>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Widgets/Button.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;

UpdateAvailableBox::UpdateAvailableBox( QString text, QWidget *parent ) :
    QDialog( parent, Qt::Window |  Qt::Dialog | Qt::FramelessWindowHint ),
    _ui( new Ui::UpdateAvailableBox ) {
    setAttribute( Qt::WA_TranslucentBackground, true );
    _ui->setupUi( this );
    _ui->_text->setText( text );
    //_ui->_text->setAlignment(Qt::AlignLeft);
    _backgroundImage = QPixmap( Constants::PLAYERUPDATECHECKER_IMAGE_DIR
                                + Constants::PLAYERUPDATECHECKER_BACKGROUND_IMAGE );

    if( _ui->_containerWidget != NULL ) {
        QBoxLayout *layout = qobject_cast<QBoxLayout *>( _ui->_containerWidget->layout() );

        if( layout != NULL ) {
            QLabel *img = new QLabel( this );
            img->setPixmap( QPixmap( Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_ICON ) );
            layout->insertWidget( 0, img );
            Button *yes = new Button( this,
                                      Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_YES,
                                      Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_YES_PRESSED,
                                      Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_YES_PRESSED );
            layout->insertWidget( 2, yes );
            connect( yes, SIGNAL( clicked() ), this, SLOT( yesClicked() ) );
            Button *later = new Button( this,
                                        Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_LATER,
                                        Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_LATER_PRESSED,
                                        Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_LATER_PRESSED );
            layout->insertWidget( 4, later );
            connect( later, SIGNAL( clicked() ), this, SLOT( laterClicked() ) );
            Button *no = new Button( this,
                                     Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_NO,
                                     Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_NO_PRESSED,
                                     Constants::PLAYERUPDATECHECKER_IMAGE_DIR + Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_NO_PRESSED );
            layout->insertWidget( 6, no );
            connect( no, SIGNAL( clicked() ), this, SLOT( noClicked() ) );
        }
    }
}

UpdateAvailableBox::~UpdateAvailableBox() {
    delete _ui;
}

void UpdateAvailableBox::yesClicked() {
    done( 1 );
}

void UpdateAvailableBox::laterClicked() {
    done( 2 );
}

void UpdateAvailableBox::noClicked() {
    done( 0 );
}

void UpdateAvailableBox::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 2, height() + 1 );
}

void UpdateAvailableBox::changeEvent( QEvent *e ) {
    QDialog::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}
