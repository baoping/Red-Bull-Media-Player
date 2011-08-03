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
#include "ScanedDirectorySettingView.h"
#include "ui_ScanedDirectorySettingView.h"


#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::StyleFactory;

ScanedDirectorySettingView::ScanedDirectorySettingView( QWidget *parent, QUuid guid ) :
    ISettingsScanDirectorySettingView( parent ),
    _ui( new Ui::ScanedDirectorySettingView ) {
    _ui->setupUi( this );
    _guid = guid;
    _removeButton = new CommandButton( this, Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR +
                                       Constants::SETTINGS_DETAIL_VIEW_REMOVE_BUTTON );
    _removeButton->setDisabledImage( Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR +
                                     Constants::SETTINGS_DETAIL_VIEW_REMOVE_BUTTON_DISABLED );
    _ui->_buttonsContainer->layout()->addWidget( _removeButton );
    connect( _removeButton, SIGNAL( clicked() ), this, SLOT( removedClickedSlot() ) );
}

ScanedDirectorySettingView::~ScanedDirectorySettingView() {
    delete _ui;
}


void ScanedDirectorySettingView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void ScanedDirectorySettingView::keyReleaseEvent( QKeyEvent *e ) {
    if( e->key() == Qt::Key_Left || e->key() == Qt::Key_Right ) {
        if( _ui->_textBox->hasFocus() )
            e->accept();
    } else
        e->ignore();
}

void ScanedDirectorySettingView::removedClickedSlot() {
    emit removeClicked( _guid );
}

void ScanedDirectorySettingView::setValue( QString value ) {
    _ui->_textBox->setText( value );
}

QString ScanedDirectorySettingView::getValue() {
    return _ui->_textBox->text();
}

void ScanedDirectorySettingView::setReadOnly( bool readOnly ) {
    _ui->_textBox->setReadOnly( readOnly );
    _removeButton->setDisabled( readOnly );
}

QUuid ScanedDirectorySettingView::guid() const {
    return _guid;
}

void ScanedDirectorySettingView::setTextBoxStyle( ITextStyle *textBoxStyle ) {
    QString textBoxStyleSheet = _ui->_textBox->styleSheet();
    textBoxStyleSheet += textBoxStyle->styleSheet();
    _ui->_textBox->setStyleSheet( textBoxStyleSheet );
}
