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
#include "ContentListView.h"
#include "ui_ContentListView.h"

#include <QBoxLayout>
#include <QScrollBar>

#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

ContentListView::ContentListView( int scrollBarWidth, QWidget *parent ) :
    IContentListView( parent ),
    _ui( new Ui::ContentListView ) {
    _ui->setupUi( this );
    _ui->_headerDateLabel->setFixedHeight( 0 );
    _ui->_headerItemLabel->setFixedHeight( 0 );
    _ui->_headerSizeLabel->setFixedHeight( 0 );
    _ui->_sizeDateHeaderSpacer->changeSize( 8, 0,
                                            QSizePolicy::Fixed, QSizePolicy::Fixed );
    _scrollBarWidth = scrollBarWidth;
    _firstHeaderWidth = -1;
    styleScrollBar();
}

ContentListView::~ContentListView() {
    delete _ui;
}

void ContentListView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void ContentListView::addContentItemView( int index, IContentItemView *view ) {
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_itemsContainer->layout() );

    if( layout == NULL )
        _ui->_itemsContainer->addWidget( view );
    else
        layout->insertWidget( index, view );

    update();
}

void ContentListView::removeContentItemView( IContentItemView *view ) {
    _ui->_itemsContainer->removeWidget( view );
    update();
}

void ContentListView::addButton( QWidget *button ) {
    _ui->_buttonsContainer->addWidget( button );
}

void ContentListView::setHeader( QString itemTitle, QString sizeTitle, QString dateTitle ) {
    _ui->_headerItemLabel->setText( itemTitle );
    _ui->_headerItemLabel->setFixedHeight( 25 );
    _ui->_headerSizeLabel->setText( sizeTitle );
    _ui->_headerSizeLabel->setFixedHeight( 25 );
    _ui->_headerDateLabel->setText( dateTitle );
    _ui->_headerDateLabel->setFixedHeight( 25 );
    _ui->_topSpacer->changeSize( 400, 5, QSizePolicy::Preferred, QSizePolicy::Fixed );
    _ui->_buttonsSpacer->changeSize( 400, 0, QSizePolicy::Preferred, QSizePolicy::Fixed );
    _ui->_sizeDateHeaderSpacer->changeSize( 8, 15,
                                            QSizePolicy::Fixed, QSizePolicy::Fixed );
}

void ContentListView::setFirstHeaderWidth( int width ) {
    _ui->_headerItemLabel->setFixedWidth( width );
    _firstHeaderWidth = width;
    QScrollBar *scrollBar = ( _ui->scrollArea->verticalScrollBar() );
    _scrollBarMinumumNotVisible = scrollBar->minimum();
    _scrollBarMaximumNotVisible = scrollBar->maximum();
    connect( scrollBar, SIGNAL( rangeChanged( int, int ) ),
             this, SLOT( resizeFirstHeaderOnScrollBarRangeChanged( int, int ) ) );
}

void ContentListView::setHeaderStyle( ITextStyle *headerStyle ) {
    if( headerStyle == NULL )
        return;

    _ui->_headerItemLabel->setStyleSheet( headerStyle->styleSheet() );
    _ui->_headerSizeLabel->setStyleSheet( headerStyle->styleSheet() );
    _ui->_headerDateLabel->setStyleSheet( headerStyle->styleSheet() );
}

void ContentListView::resizeFirstHeaderOnScrollBarRangeChanged( int minimum, int maximum ) {
    if( minimum == _scrollBarMinumumNotVisible && maximum == _scrollBarMaximumNotVisible )
        _ui->_headerItemLabel->setFixedWidth( _firstHeaderWidth );
    else
        _ui->_headerItemLabel->setFixedWidth( _firstHeaderWidth - _scrollBarWidth );
}

void ContentListView::styleScrollBar() {
    QString scrollBarStyleSheet =  "QScrollBar:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_background);"
                                   "width: 18px;"
                                   "margin: 16px 0 16px 0;"
                                   "}"
                                   "QScrollBar::handle:vertical {"
                                   "background-image: url(:/scroll_slider);"
                                   "min-height: 20px;"
                                   "margin-left:2px;"
                                   "}"
                                   "QScrollBar::add-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_down);"
                                   "height:16px;"
                                   "subcontrol-position: bottom;"
                                   "subcontrol-origin: margin;"
                                   "margin-left:1px;"
                                   "}"
                                   "QScrollBar::sub-line:vertical {"
                                   "border: 0px;"
                                   "background-image: url(:/scroll_up);"
                                   "height: 16px;"
                                   "subcontrol-position: top;"
                                   "subcontrol-origin: margin;"
                                   "margin-left:1px;"
                                   "}"
                                   "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                   "border: 0px;"
                                   "width: 0px;"
                                   "height: 0px;"
                                   "background: transparent;"
                                   "}"
                                   "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                   "background: none;"
                                   "}"
                                   "QWidget{background: transparent;}";
    setStyleSheet( scrollBarStyleSheet );
    _ui->scrollArea->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
}
