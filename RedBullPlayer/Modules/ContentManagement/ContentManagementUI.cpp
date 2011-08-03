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
#include "ContentManagementUI.h"
#include "ui_ContentManagementUI.h"

#include <QPainter>
#include <QFontMetrics>
#include <QFont>

using namespace RedBullPlayer::Modules::StyleFactory;

ContentManagementUI::ContentManagementUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::ContentManagementUI ) {
    _ui->setupUi( this );
    setWindowAttributes();
}

ContentManagementUI::~ContentManagementUI() {
    delete _ui;
}

void ContentManagementUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void ContentManagementUI::setWindowAttributes() {
    setWindowFlags( Qt::Tool | Qt::FramelessWindowHint );
    setAttribute( Qt::WA_QuitOnClose );
    setAttribute( Qt::WA_MacAlwaysShowToolWindow, false );
    setAttribute( Qt::WA_TranslucentBackground, true );
}

void ContentManagementUI::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 1, height() + 1 );
}

void ContentManagementUI::setTabTitles( QString availableTitle, QString downloadedTitle, QString skippedTitle ) {
    _ui->_tabWidget->setTabText( 0, availableTitle );
    _ui->_tabWidget->setTabText( 1, downloadedTitle );
    _ui->_tabWidget->setTabText( 2, skippedTitle );
    _ui->_tabWidget->setCurrentIndex( 0 );
}

void ContentManagementUI::addAvailableListView( QWidget *listView ) {
    _ui->_availableTab->layout()->addWidget( listView );
}

void ContentManagementUI::addDownloadedListView( QWidget *listView ) {
    _ui->_downloadedTab->layout()->addWidget( listView );
}

void ContentManagementUI::addSkippedListView( QWidget *listView ) {
    _ui->_skippedTab->layout()->addWidget( listView );
}

void ContentManagementUI::enterEvent( QEvent * ) {
    emit mouseEnter();
}

void ContentManagementUI::leaveEvent( QEvent * ) {
    emit mouseLeave();
}

void ContentManagementUI::show( int x, int y ) {
    int negativeOffsetXCircleCountDisplay = 25;

    if( !isVisible() ) {
        QWidget::move( x - negativeOffsetXCircleCountDisplay, y );
        QWidget::show();
    } else {
        QWidget::move( x - negativeOffsetXCircleCountDisplay, y );
    }
}

void ContentManagementUI::hide() {
    if( isVisible() )
        QWidget::hide();
}

void ContentManagementUI::setTabTitlesStyle( ITextStyle *titleStyle ) {
    if( titleStyle == NULL )
        return;

    QString fontWeight = "";
    QFont::Weight fontBold = QFont::Normal;

    if( titleStyle->bold() ) {
        fontWeight = "font-weight:bold;";
        fontBold = QFont::Bold;
    }

    QFont titleFont = QFont( titleStyle->fontName(), titleStyle->pointSize(), fontBold, titleStyle->italic() );
    titleFont.setPixelSize( titleStyle->pointSize() );
    QFontMetrics fontMetrics = QFontMetrics( titleFont );
    QString tabStyleSheet = "QTabWidget::pane {"
                            "border: none;"
                            "background-image:url(\":/backgroundContent\");"
                            "background-position:bottom;"
                            "background-repeat:no-repeat;"
                            "border-radius:4;"
                            "}"
                            "QTabWidget::tab-bar {"
                            "left: 20px;"
#if defined(Q_OS_MAC)
                            "top:-4px;"
#else
                            "top:-1px;"
#endif
                            "}"
                            "QTabBar::tab {"
                            "background:transparent;"
                            "border: none;"
                            "border-top-left-radius: 4px;"
                            "border-top-right-radius: 4px;"
                            "padding: 3px 10px 4px 10px;"
                            "color:" + titleStyle->rgbaColorString() + ";"
                            "font-size:" + QString::number( titleStyle->pointSize() ) + "px;"
                            "font-family:" + titleStyle->fontName() + ";"
                            + fontWeight +
                            "margin-right:4px;"
                            "}"
                            "QTabBar::tab:selected, QTabBar::tab:hover {"
                            "background: rgba(227,0,63,90%);"
                            "}"
                            "QTabBar::tab:!selected {"
                            "background-color:rgba(0,111,173,90%);"
                            "}"
                            "QTabBar::tab:first{"
                            "width:" + QString::number( fontMetrics.width( _ui->_tabWidget->tabText( 0 ) ) ) + "px;"
                            "}"
                            "QTabBar::tab:middle{"
                            "width:" + QString::number( fontMetrics.width( _ui->_tabWidget->tabText( 1 ) ) ) + "px;"
                            "}"
                            "QTabBar::tab:last{"
                            "width:" + QString::number( fontMetrics.width( _ui->_tabWidget->tabText( 2 ) ) ) + "px;"
                            "}";
    _ui->_tabWidget->setStyleSheet( tabStyleSheet );
}
