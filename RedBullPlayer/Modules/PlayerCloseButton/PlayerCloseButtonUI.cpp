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
#include "PlayerCloseButtonUI.h"
#include "ui_PlayerCloseButtonUI.h"

#include "../../Container/Constants.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;

PlayerCloseButtonUI::PlayerCloseButtonUI( Shell *shell, QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::PlayerCloseButtonUI ) {
    _ui->setupUi( this );
    _closeButton = new CommandButton( parent,
                                      Constants::PLAYLER_CLOSE_BUTTON_IMAGE_DIR + Constants::PLAYLER_CLOSE_BUTTON_X_URL );
    _closeButton->setMouseOverImage( Constants::PLAYLER_CLOSE_BUTTON_IMAGE_DIR + "active" + Constants::PLAYLER_CLOSE_BUTTON_X_URL );
    connect( _closeButton, SIGNAL( clicked() ), this, SLOT( playerCloseButtonClickedSlot() ) );
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_buttonsContainer->layout() );

    if( layout == NULL ) {
        Error error;
        error.setErrorMessage( tr( "Cannot display playerclose buttons because buttonContainer layout cannot be cast to QBoxLayout" ), Error::Fatal );
        Error::raise( &error );
        return;
    }

    layout->insertWidget( 0, _closeButton );
}

PlayerCloseButtonUI::~PlayerCloseButtonUI() {
    delete _ui;
}

void PlayerCloseButtonUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void PlayerCloseButtonUI::playerCloseButtonClickedSlot() {
    emit playerCloseButtonClicked();
}
