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
#include "IntegerSettingView.h"
#include "ui_IntegerSettingView.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::StyleFactory;

IntegerSettingView::IntegerSettingView( QWidget *parent, QUuid guid, QString text ) :
    ISettingsIntegerSettingView( parent ),
    _ui( new Ui::IntegerSettingView ) {
    _ui->setupUi( this );
    _guid = guid;
    _ui->_label->setText( text );
}

IntegerSettingView::~IntegerSettingView() {
    delete _ui;
}

void IntegerSettingView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void IntegerSettingView::keyReleaseEvent( QKeyEvent *e ) {
    if( e->key() == Qt::Key_Left || e->key() == Qt::Key_Right ) {
        if( _ui->_spinBox->hasFocus() )
            e->accept();
    } else
        e->ignore();
}

QUuid IntegerSettingView::guid() const {
    return _guid;
}

void IntegerSettingView::setLabelStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *labelStyle ) {
    if( labelStyle == NULL )
        return;

    QString labelStyleSheet = labelStyle->styleSheet();
    labelStyleSheet += "border:none;";
    _ui->_label->setStyleSheet( labelStyleSheet );
}

void IntegerSettingView::setValue( int value ) {
    _ui->_spinBox->setValue( value );
}

int IntegerSettingView::getValue() {
    return _ui->_spinBox->value();
}
