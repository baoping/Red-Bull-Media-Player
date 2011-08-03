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
#include "BooleanSettingView.h"
#include "ui_BooleanSettingView.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::StyleFactory;

BooleanSettingView::BooleanSettingView( QWidget *parent, QUuid guid, QString text ) :
    ISettingsBooleanSettingView( parent ),
    _ui( new Ui::BooleanSettingView ) {
    _ui->setupUi( this );
    _guid = guid;
    _ui->_checkBox->setText( text );
    connect( _ui->_checkBox, SIGNAL( toggled( bool ) ), this, SIGNAL( valueChanged( bool ) ) );
    styleCheckBox();
}

BooleanSettingView::~BooleanSettingView() {
    delete _ui;
}

void BooleanSettingView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

QUuid BooleanSettingView::guid() const {
    return _guid;
}

void BooleanSettingView::setLabelStyle( ITextStyle *labelStyle ) {
    if( labelStyle == NULL )
        return;

    QString checkBoxStyleSheet = _ui->_checkBox->styleSheet();
    checkBoxStyleSheet += ( "QCheckBox {" + labelStyle->styleSheet() + "}" );
    _ui->_checkBox->setStyleSheet( checkBoxStyleSheet );
}

void BooleanSettingView::setValue( bool value ) {
    _ui->_checkBox->setChecked( value );
}

void BooleanSettingView::styleCheckBox() {
    QString checkBoxStyleSheet = "QCheckBox {"
                                 "spacing: 10px;"
                                 "font-size:16px;"
                                 "color:white;"
                                 "}"
                                 "QCheckBox::indicator {"
                                 "width: 13px;"
                                 "height: 13px;"
                                 "}"
                                 "QCheckBox::indicator:unchecked {"
                                 "image: url(:/unchecked);"
                                 "}"
                                 "QCheckBox::indicator:unchecked:hover {"
                                 "image: url(:/unchecked);"
                                 "}"
                                 "QCheckBox::indicator:unchecked:pressed {"
                                 "image: url(:/unchecked);"
                                 "}"
                                 "QCheckBox::indicator:checked {"
                                 "image: url(:/checked);"
                                 "}"
                                 "QCheckBox::indicator:checked:hover {"
                                 "image: url(:/checked);"
                                 "}"
                                 "QCheckBox::indicator:checked:pressed {"
                                 "image: url(:/checked);"
                                 " }"
                                 "QCheckBox::indicator:indeterminate:hover {"
                                 "image: url(:/unchecked);"
                                 " }"
                                 " QCheckBox::indicator:indeterminate:pressed {"
                                 "image: url(:/unchecked);"
                                 "}";
    _ui->_checkBox->setStyleSheet( checkBoxStyleSheet );
}
