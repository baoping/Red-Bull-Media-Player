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
#include "DownloadedView.h"
#include "ui_DownloadedView.h"

#include <QFontMetrics>

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::StyleFactory;

DownloadedView::DownloadedView( QString guid, QWidget *parent ) :
    IContentItemView( guid, parent ),
    _ui( new Ui::DownloadedView ) {
    _ui->setupUi( this );
    _titleText = "";
}

DownloadedView::~DownloadedView() {
    delete _ui;
}

void DownloadedView::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void DownloadedView::setTextStyle( ITextStyle *textStyle ) {
    if( textStyle == NULL )
        return;

    QString titleLabelStyleSheet = "QToolTip { background-color: #FBFAFD; font: Tahoma; color: black;}";
    titleLabelStyleSheet += "\nQLabel{";
    titleLabelStyleSheet += textStyle->styleSheet();
    titleLabelStyleSheet += "}";
    _ui->_titleLabel->setStyleSheet( titleLabelStyleSheet );
    _ui->_sizeLabel->setStyleSheet( textStyle->styleSheet() );
    _ui->_dateLabel->setStyleSheet( textStyle->styleSheet() );
}

void DownloadedView::setTitle( QString title ) {
    _titleText = title;
    QFontMetrics fontMetrics = QFontMetrics( _ui->_titleLabel->font() );
    QString elidedTitle = fontMetrics.elidedText( title, Qt::ElideRight, _ui->_titleLabel->width() );
    _ui->_titleLabel->setText( elidedTitle );
}

void DownloadedView::setSize( QString sizeString ) {
    _ui->_sizeLabel->setText( sizeString );
}

void DownloadedView::setDate( QString dateString ) {
    _ui->_dateLabel->setText( dateString );
}

void DownloadedView::setToolTipText( QString toolTip ) {
    _ui->_titleLabel->setToolTip( toolTip );
}
