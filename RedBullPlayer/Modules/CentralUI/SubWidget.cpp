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
#include "SubWidget.h"
#include "ui_SubWidget.h"

SubWidget::SubWidget( RedBullPlayer::PlayerShell::Shell* shell,
                      QWidget *parent ) :
    QWidget( parent, Qt::Tool | Qt::FramelessWindowHint ),
    ui( new Ui::SubWidget ) {
    ui->setupUi( this );
    _shell = shell;
    setAttribute( Qt::WA_QuitOnClose );
    setAttribute( Qt::WA_MacAlwaysShowToolWindow, false );
    setAttribute( Qt::WA_AlwaysShowToolTips, true );
}

SubWidget::~SubWidget() {
    delete ui;
}

void SubWidget::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SubWidget::keyReleaseEvent ( QKeyEvent * event ) {
    _shell->invokeKeyReleaseEvent( event );
}


void SubWidget::keyPressEvent( QKeyEvent * ) {}
