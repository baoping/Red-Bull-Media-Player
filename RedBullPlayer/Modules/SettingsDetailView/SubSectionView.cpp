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
#include "SubSectionView.h"
#include "ui_SubSectionView.h"

#include <QBoxLayout>

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::StyleFactory;

SubSectionView::SubSectionView( QWidget *parent, QString title ) :
    ISettingsSubSectionView( parent ),
    _ui( new Ui::SubSectionView ) {
    _ui->setupUi( this );

    if( title == "" ) {
        _ui->_titleLabel->setFixedHeight( 0 );
    } else
        _ui->_titleLabel->setText( title );
}

SubSectionView::~SubSectionView() {
    delete _ui;
}

void SubSectionView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SubSectionView::addSettingView( ISettingsSettingView *settingView ) {
    _ui->_settingsContainer->layout()->addWidget( settingView );
}

void SubSectionView::removeSettingView( ISettingsSettingView *settingView ) {
    _ui->_settingsContainer->layout()->removeWidget( settingView );
    update();
}

void SubSectionView::insertSettingView( int atIndex, ISettingsSettingView *settingView ) {
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_settingsContainer->layout() );
    layout->insertWidget( atIndex, settingView );
    update();
}

void SubSectionView::setTitleStyle( ITextStyle *titleStyle ) {
    if( titleStyle == NULL )
        return;

    QString titleStyleSheet = titleStyle->styleSheet();
    titleStyleSheet += "border:none;";
    _ui->_titleLabel->setStyleSheet( titleStyleSheet );
}
