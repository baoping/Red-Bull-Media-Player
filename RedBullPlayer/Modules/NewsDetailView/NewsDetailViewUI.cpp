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
#include "NewsDetailViewUI.h"
#include "ui_NewsDetailViewUI.h"

// Qt Includes
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QString>
#include <QTimer>
#include <QScrollBar>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Container.h"
#include "../../Container/Error.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;

NewsDetailViewUI::NewsDetailViewUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::NewsDetailViewUI ) {
    resolveMediaLibrary();
    _ui->setupUi( this );
    _browser = new TextBrowser( _mediaLibrary, this );
    _browser->setFrameShape( QTextBrowser::NoFrame );
    _browser->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    _browser->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
    _browser->setContextMenuPolicy( Qt::NoContextMenu );
    _ui->_textBrowserContainer->layout()->addWidget( _browser );
    _closeButton = new Button( this,
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN,
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + "active" + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN,
                               Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_CLOSE_BTN );
    connect( _closeButton, SIGNAL( clicked() ), this, SIGNAL( exitClicked() ) );
    _closeButton->setClickDownDistance( 0 );
    QBoxLayout *_closeButtonContainerLayout = qobject_cast<QBoxLayout*>( _ui->_closeButtonContainer->layout() );

    if( _closeButtonContainerLayout != NULL ) {
        _closeButtonContainerLayout->insertWidget( 0, _closeButton );
    }
}

NewsDetailViewUI::~NewsDetailViewUI() {
    delete _ui;
}

void NewsDetailViewUI::paintEvent( QPaintEvent */*e*/ ) {
    QPainter painter( this );
    QImage origImg = QImage( Constants::NEWS_DETAIL_VIEW_IMAGE_DIR + Constants::NEWS_DETAIL_VIEW_BG_URL );
    QImage img = origImg.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    painter.drawImage( 0, 0, img );
}

void NewsDetailViewUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void NewsDetailViewUI::setNewsGuid( QUuid newsGuid ) {
    if ( _ui == 0 ) {
        return;
    }

    _browser->setNewsGuid( newsGuid );
}

void NewsDetailViewUI::setTitle( QString title ) {
    _ui->_title->setText( title );
}

void NewsDetailViewUI::setPubDate( QString pubDate ) {
    _ui->_pubDate->setText( pubDate );
}

void NewsDetailViewUI::setLink( QString url ) {
    _ui->_link->setText( QString( "<qt><a style=\"%3\" href=\"%2\">%1</a></qt>" )
                         .arg( Constants::NEWS_DETAIL_VIEW_LINK_TEXT )
                         .arg( url )
                         .arg( _linkStyle ) );
}

void NewsDetailViewUI::setDescription( QString description ) {
    _ui->_descriptionLabel->setVisible( description != "" );
    _ui->_descriptionLabel->setText( description );
}

void NewsDetailViewUI::setText( QString text ) {
    QFile file( Constants::NEWS_DETAIL_VIEW_TEXT_DIR + Constants::NEWS_DETAIL_VIEW_STYLE );
    file.open( QFile::ReadOnly );
    QByteArray style = file.readAll();
    file.close();
    QString styleTemplate = QString( style );
    styleTemplate = styleTemplate.replace( "{#FeedTitle}", _ui->_title->text() );
    styleTemplate = styleTemplate.replace( "{#Feed}", text );
    _browser->setHtml( styleTemplate );
    //_ui->_text->setHtml( text );
    //_ui->_text->setText(text);
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, "NewsDetail html : "
                                     + _browser->toHtml() );
}

void NewsDetailViewUI::enterEvent( QEvent */*e*/ ) {
    MainUIRegionFocusChangedEvent event( MainUIRegionFocusChangedEvent::CENTRAL );
    Aggregator::instance()->sendEvent( &event );
}

void NewsDetailViewUI::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void NewsDetailViewUI::resolveMediaLibrary() {
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

void NewsDetailViewUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    setupTitleStyle();
    setupPublishDateStyle();
    setupLinkStyle();
    setupDescriptionStyle();
}

void NewsDetailViewUI::setupDescriptionStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "NewsDetailView_DescriptionText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color:transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_descriptionLabel->setStyleSheet( titleStyle );
        _ui->_descriptionLabel->setAlignment( titleTextStyle->alignment() );
    }
}

void NewsDetailViewUI::setupTitleStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "NewsDetailView_TitleText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color:transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_title->setStyleSheet( titleStyle );
        _ui->_title->setAlignment( titleTextStyle->alignment() );
    }
}

void NewsDetailViewUI::setupPublishDateStyle() {
    bool ok;
    ITextStyle *publishingDateTextStyle = _styleFactory->getTextStyle( "NewsDetailView_PublishingDateText", &ok );

    if( ok ) {
        QString publishingDateStyle = QString( "background-color:transparent;" );
        publishingDateStyle += publishingDateTextStyle->styleSheet();
        _ui->_pubDate->setStyleSheet( publishingDateStyle );
        _ui->_pubDate->setAlignment( publishingDateTextStyle->alignment() );
    }
}

void NewsDetailViewUI::setupLinkStyle() {
    _linkStyle = "";
    bool ok;
    ITextStyle *linkTextStyle = _styleFactory->getTextStyle( "NewsDetailView_LinkText", &ok );

    if( ok ) {
        _linkStyle += linkTextStyle->styleSheet();
    }
}
