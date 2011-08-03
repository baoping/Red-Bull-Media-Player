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
#include "AboutDetailViewUI.h"
#include "ui_AboutDetailViewUI.h"

// Qt Includes
#include <QDebug>
#include <QPainter>
#include <QScrollBar>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::MediaLibrary;

AboutDetailViewUI::AboutDetailViewUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::AboutDetailViewUI ) {
    _ui->setupUi( this );
    resolveMediaLibrary();
    _browser = new TextBrowser( _mediaLibrary, this );
    _browser->setFrameShape( QTextBrowser::NoFrame );
    _browser->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    _browser->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
    _browser->setContextMenuPolicy( Qt::NoContextMenu );
    _ui->_browserContainer->layout()->addWidget( _browser );
}

AboutDetailViewUI::~AboutDetailViewUI() {
    delete _ui;
}

void AboutDetailViewUI::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );
    QImage origImg = QImage( Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_BG_URL );
    QImage img = origImg.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    painter.drawImage( 0, 0, img );
}

void AboutDetailViewUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void AboutDetailViewUI::setTitle( QString title ) {
    _ui->_title->setText( title );
}

void AboutDetailViewUI::setText( QString text ) {
    QFile file( Constants::ABOUT_DETAIL_VIEW_TEXT_DIR + Constants::ABOUT_DETAIL_VIEW_STYLE );
    file.open( QFile::ReadOnly );
    QByteArray style = file.readAll();
    file.close();
    QString styleTemplate = QString( style );
    styleTemplate = styleTemplate.replace( "{#Title}", _ui->_title->text() );
    styleTemplate = styleTemplate.replace( "{#Body}", text );
    _browser->setHtml( styleTemplate );
}

void AboutDetailViewUI::enterEvent( QEvent */*e*/ ) {
    emitMainRegionFocusChanged();
}

void AboutDetailViewUI::resolveStyleFactory() {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, Q_FUNC_INFO );
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
        SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, "Resolving succeded" );
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void AboutDetailViewUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    setupTitleStyle();
}

void AboutDetailViewUI::setupTitleStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "AboutBox_HeaderText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color:transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_title->setStyleSheet( titleStyle );
        _ui->_title->setAlignment( titleTextStyle->alignment() );
    }
}

void AboutDetailViewUI::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

