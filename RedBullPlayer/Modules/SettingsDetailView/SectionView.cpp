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
#include "SectionView.h"
#include "ui_SectionView.h"

#include "../../Widgets/HelpWidget.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

SectionView::SectionView( QString title, QString help, QWidget *parent ) :
    ISettingsSectionView( parent ),
    _ui( new Ui::SectionView ) {
    _ui->setupUi( this );
    _ui->_titleLabel->setText( title );
    HelpWidget *helpWidget = new HelpWidget( this, Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_HELP_ICON,
            Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_HELP_ICON_HOVER,
            help );
    QBoxLayout *layout = qobject_cast<QBoxLayout *>( _ui->_headerContainer->layout() );

    if( layout != NULL )
        layout->insertWidget( 2, helpWidget );
}

SectionView::~SectionView() {
    delete _ui;
}

void SectionView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SectionView::addSubSectionView( ISettingsSubSectionView *subSection ) {
    _ui->_subSectionsContainer->addWidget( subSection );
}

void SectionView::setTitleStyle( ITextStyle *titleStyle ) {
    if( titleStyle == NULL )
        return;

    QString titleStyleSheet = titleStyle->styleSheet();
    titleStyleSheet += "border:none;";
    _ui->_titleLabel->setStyleSheet( titleStyleSheet );
}
