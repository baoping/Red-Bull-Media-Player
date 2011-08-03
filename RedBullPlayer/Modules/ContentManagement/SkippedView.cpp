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
#include "SkippedView.h"
#include "ui_SkippedView.h"

#include <QFontMetrics>

#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;

SkippedView::SkippedView( QString guid, QWidget *parent ) :
    IContentItemView( guid, parent ),
    _ui( new Ui::SkippedView ) {
    _ui->setupUi( this );
    _downloadButton = new CommandButton( this, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON );
    _downloadButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON_PRESSED );
    _deleteButton = new CommandButton( this, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DELETE_BUTTON );
    _deleteButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DELETE_BUTTON_PRESSED );
    connect( _downloadButton, SIGNAL( clicked() ), this, SLOT( downloadClickedSlot() ) );
    connect( _deleteButton, SIGNAL( clicked() ), this, SLOT( deleteClickedSlot() ) );
    _ui->_buttonsContainer->addWidget( _downloadButton );
    _ui->_buttonsContainer->addWidget( _deleteButton );
    _titleText = "";
}

SkippedView::~SkippedView() {
    delete _ui;
}

void SkippedView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SkippedView::resizeEvent( QResizeEvent * ) {
    setTitle( _titleText );
}

void SkippedView::downloadClickedSlot() {
    emit downloadClicked( guid() );
}

void SkippedView::deleteClickedSlot() {
    emit deleteClicked( guid() );
}

void SkippedView::setTextStyle( ITextStyle *textStyle ) {
    if( textStyle == NULL )
        return;

    QString titleLabelStyleSheet = "QToolTip { background-color: #FBFAFD; font: Tahoma; color: black;}";
    titleLabelStyleSheet += "\nQLabel{";
    titleLabelStyleSheet += textStyle->styleSheet();
    titleLabelStyleSheet += "}";
    _ui->_titleLabel->setStyleSheet( titleLabelStyleSheet );
    _ui->_sizeLabel->setStyleSheet( textStyle->styleSheet() );
}

void SkippedView::setTitle( QString title ) {
    _titleText = title;
    QFontMetrics fontMetrics = QFontMetrics( _ui->_titleLabel->font() );
    QString elidedTitle = fontMetrics.elidedText( title, Qt::ElideRight, _ui->_titleLabel->width() );
    _ui->_titleLabel->setText( elidedTitle );
}

void SkippedView::setSize( QString sizeString ) {
    _ui->_sizeLabel->setText( sizeString );
}
void SkippedView::setToolTipText( QString toolTip ) {
    _ui->_titleLabel->setToolTip( toolTip );
}
