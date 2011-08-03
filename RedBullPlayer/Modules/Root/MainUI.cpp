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
#include "MainUI.h"
#include "ui_MainUI.h"

// Qt Includes
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QPainter>


// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerStoppedEvent.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"



using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

MainUI::MainUI( Shell* shell, QWidget *parent ) :
    QWidget( parent ),
    m_ui( new Ui::MainUI ) {
    Q_ASSERT( shell != 0 );
    _shell = shell;
    m_ui->setupUi( this );

    //NewsTicker ON OFF Code
    if( !ConfigManager::instance()->newsTickerOn() )
        m_ui->bottomRegionFrame->setFixedHeight( 103 );
    else
        m_ui->bottomRegionFrame->setFixedHeight( 143 );

    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "TopRegion", m_ui->topRegionFrame ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "SearchRegion", m_ui->searchRegionFrame ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "CentralRegion", m_ui->centerRegionFrame ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "BottomRegion", m_ui->bottomRegionFrame ) );
}

MainUI::~MainUI() {
    delete m_ui;
}

void MainUI::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );
}

void MainUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

