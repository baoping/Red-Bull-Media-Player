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
#include "AvailableDownloadView.h"
#include "ui_AvailableDownloadView.h"

#include <QBoxLayout>
#include <QFontMetrics>

#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;

AvailableDownloadView::AvailableDownloadView( QString guid, QWidget *parent ) :
    IContentItemView( guid, parent ),
    _ui( new Ui::AvailableDownloadView ) {
    _ui->setupUi( this );

    _downloadButton = new CommandButton( this, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON );
    _downloadButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON_PRESSED );
    connect( _downloadButton, SIGNAL( clicked() ), this, SLOT( downloadClicked() ) );

    _skipButton = new CommandButton( this, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_SKIP_BUTTON );
    _skipButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_SKIP_BUTTON_PRESSED );
    connect( _skipButton, SIGNAL( clicked() ), this, SLOT( skipClicked() ) );

    _cancelButton = new CommandButton( this, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_CANCEL_BUTTON );
    _cancelButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR + Constants::CONTENT_MAMAGEMENT_CANCEL_BUTTON_PRESSED );
    connect( _cancelButton, SIGNAL( clicked() ), this, SLOT( cancelClicked() ) );

    _ui->_buttonsContainer->addWidget( _downloadButton );
    _ui->_buttonsContainer->addWidget( _skipButton );
    _cancelButton->hide();
    _ui->_progressBar->hide();
    _ui->_progressBarSpacer->changeSize( _ui->_progressBar->width(), 20, QSizePolicy::Fixed, QSizePolicy::Preferred );
    _titleText = "";
    _labelsMarginRight = 3;
}

AvailableDownloadView::~AvailableDownloadView() {
    delete _ui;
}

void AvailableDownloadView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void AvailableDownloadView::resizeEvent( QResizeEvent * ) {
    setTitle( _titleText );
}

void AvailableDownloadView::onDownloadStarted() {
    _ui->_progressBarSpacer->changeSize( 0, 20, QSizePolicy::Fixed, QSizePolicy::Preferred );
    _ui->_progressBar->setValue( 0 );
    _ui->_progressBar->show();
    _ui->_buttonsContainer->removeWidget( _downloadButton );
    QHBoxLayout *layout = qobject_cast<QHBoxLayout*>( _ui->_buttonsContainer->layout() );

    if( layout == NULL )
        _ui->_buttonsContainer->addWidget( _cancelButton );
    else
        layout->insertWidget( 0, _cancelButton );

    _downloadButton->hide();
    _cancelButton->show();
}

void AvailableDownloadView::setProgress( int progressValue ) {
    _ui->_progressBar->setValue( progressValue );
}

void AvailableDownloadView::onDownloadCanceled() {
    _ui->_progressBarSpacer->changeSize( _ui->_progressBar->width(), 20, QSizePolicy::Fixed, QSizePolicy::Preferred );
    _ui->_progressBar->setValue( 0 );
    _ui->_progressBar->hide();
    _ui->_buttonsContainer->removeWidget( _cancelButton );
    QHBoxLayout *layout = qobject_cast<QHBoxLayout*>( _ui->_buttonsContainer->layout() );

    if( layout == NULL )
        _ui->_buttonsContainer->addWidget( _downloadButton );
    else
        layout->insertWidget( 0, _downloadButton );

    _cancelButton->hide();
    _downloadButton->show();
}

void AvailableDownloadView::downloadClicked() {
    emit downloadSignal( guid() );
}

void AvailableDownloadView::skipClicked() {
    emit skipSignal( guid() );
}

void AvailableDownloadView::cancelClicked() {
    emit cancelSignal( guid() );
}

void AvailableDownloadView::setTextStyle( ITextStyle *textStyle ) {
    if( textStyle == NULL )
        return;

    QString labelStyleSheet = "QToolTip { background-color: #FBFAFD; font: Tahoma; color: black;}";
    labelStyleSheet += QString( "\nQLabel{margin:0 %1 0 -3;" ).arg( _labelsMarginRight );
    labelStyleSheet += textStyle->styleSheet();
    labelStyleSheet += "}";
    _ui->_titleLabel->setStyleSheet( labelStyleSheet );
    _ui->_sizeLabel->setStyleSheet( labelStyleSheet );
}

void AvailableDownloadView::setTitle( QString title ) {
    _titleText = title;
    QFontMetrics fontMetrics = QFontMetrics( _ui->_titleLabel->font() );
    QString elidedTitle = fontMetrics.elidedText( title, Qt::ElideRight,
                          _ui->_titleLabel->width() - _labelsMarginRight );
    _ui->_titleLabel->setText( elidedTitle );
}

void AvailableDownloadView::setToolTipText( QString toolTip ) {
    _ui->_titleLabel->setToolTip( toolTip );
}

void AvailableDownloadView::setSize( QString sizeString ) {
    _ui->_sizeLabel->setText( sizeString );
}
