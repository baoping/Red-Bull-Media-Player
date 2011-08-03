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
#include "FileExtensionsSettingView.h"
#include "ui_FileExtensionsSettingView.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::StyleFactory;

FileExtensionsSettingView::FileExtensionsSettingView( QWidget *parent, QUuid guid, QString labelText ) :
    ISettingsFileExtensionsSettingView( parent ),
    _ui( new Ui::FileExtensionsSettingView ) {
    _ui->setupUi( this );
    _guid = guid;
    _ui->_label->setText( labelText );
}

FileExtensionsSettingView::~FileExtensionsSettingView() {
    delete _ui;
}

void FileExtensionsSettingView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void FileExtensionsSettingView::keyReleaseEvent( QKeyEvent *e ) {
    if( e->key() == Qt::Key_Left || e->key() == Qt::Key_Right ) {
        if( _ui->_textBox->hasFocus() )
            e->accept();
    } else
        e->ignore();
}

QUuid FileExtensionsSettingView::guid() const {
    return _guid;
}

void FileExtensionsSettingView::setValue( QString value ) {
    _ui->_textBox->setText( value );
}

QString FileExtensionsSettingView::getValue() {
    return _ui->_textBox->text();
}

void FileExtensionsSettingView::setText( QString text ) {
    _ui->_label->setText( text );
}

void FileExtensionsSettingView::setLabelStyle( ITextStyle *labelStyle ) {
    if( labelStyle == NULL )
        return;

    QString labelStyleSheet = labelStyle->styleSheet();
    labelStyleSheet += "border:none;";
    _ui->_label->setStyleSheet( labelStyleSheet );
}

void FileExtensionsSettingView::setTextBoxStyle( ITextStyle *textBoxStyle ) {
    if( textBoxStyle == NULL )
        return;

    QString textBoxStyleSheet = _ui->_textBox->styleSheet();
    textBoxStyleSheet += textBoxStyle->styleSheet();
    _ui->_textBox->setStyleSheet( textBoxStyleSheet );
}
