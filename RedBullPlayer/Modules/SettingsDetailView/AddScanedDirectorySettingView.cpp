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
#include "AddScanedDirectorySettingView.h"
#include "ui_AddScanedDirectorySettingView.h"

#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::StyleFactory;

AddScanedDirectorySettingView::AddScanedDirectorySettingView( QWidget *parent, QUuid guid ) :
    ISettingsAddDirectorySettingView( parent ),
    _ui( new Ui::AddScanedDirectorySettingView ) {
    _ui->setupUi( this );
    _guid = guid;
    _addButton = new CommandButton( this, Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR +
                                    Constants::SETTINGS_DETAIL_VIEW_ADD_BUTTON );
    _ui->_buttonsContainer->layout()->addWidget( _addButton );
    connect( _addButton, SIGNAL( clicked() ), this, SIGNAL( addClicked() ) );
#if defined(Q_OS_MAC)
    _ui->_rightMarginHorizontalSpacer->changeSize( 1, 20 );
#endif
}

AddScanedDirectorySettingView::~AddScanedDirectorySettingView() {
    delete _ui;
}

void AddScanedDirectorySettingView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

QUuid AddScanedDirectorySettingView::guid() const {
    return _guid;
}


