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
#include "ChapterSelectUI.h"
#include "ui_ChapterSelectUI.h"

//Qt Includes
#include <QDebug>
#include <QPainter>

//Project Includes
#include "Chapter.h"
#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../Widgets/TransparentImage.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;

ChapterSelectUI::ChapterSelectUI( QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::ChapterSelectUI ) {
    _ui->setupUi( this );
    setAttribute( Qt::WA_TranslucentBackground, true );
    initNavigationButtons();
    _carrousel = new ChapterCarrousel( _ui->_chapterContainer );
    connect( _carrousel, SIGNAL( itemClicked( int ) ), this, SLOT( carrouselItemClickedSlot( int ) ) );
    connect( _carrousel, SIGNAL( itemsRepositioned() ), this, SLOT( itemsRepositionedSlot() ) );
    _background = QPixmap( Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_BACKGROUND_IMAGE );
}

ChapterSelectUI::~ChapterSelectUI() {
    delete _ui;
}

void ChapterSelectUI::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 1, height() + 1 );
    painter.drawPixmap( 0, 0,
                        _background.scaled( width(), height(), Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
}

void ChapterSelectUI::initNavigationButtons() {
    _left = createLeftNavigationButton();
    _right = createRightNavigationButton();
}

CommandButton* ChapterSelectUI::createLeftNavigationButton() {
    CommandButton* button = new CommandButton( _ui->_leftNavigationContainer,
            Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_LEFT_ARROW_URL,
            Constants::CHAPTER_SELECT_ARROW_WIDTH, Constants::CHAPTER_SELECT_ARROW_HEIGHT );
    button->setDisabledImage( Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_LEFT_ARROW_DISABLED_URL );
    connect( button, SIGNAL( clicked() ), this, SLOT( onLeftNavigationClicked() ) );
    QBoxLayout *leftLayout = qobject_cast<QBoxLayout*>( _ui->_leftNavigationContainer->layout() );

    if( leftLayout != NULL ) {
        leftLayout->insertWidget( 1, button );
    }

    return button;
}

CommandButton* ChapterSelectUI::createRightNavigationButton() {
    CommandButton* button = new CommandButton( _ui->_rightNavigationContainer,
            Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_RIGHT_ARROW_URL,
            Constants::CHAPTER_SELECT_ARROW_WIDTH, Constants::CHAPTER_SELECT_ARROW_HEIGHT );
    button->setDisabledImage( Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_RIGHT_ARROW_DISABLED_URL );
    connect( button, SIGNAL( clicked() ), this, SLOT( onRightNavigationClicked() ) );
    QBoxLayout *rightLayout = qobject_cast<QBoxLayout*>( _ui->_rightNavigationContainer->layout() );

    if( rightLayout != NULL ) {
        rightLayout->insertWidget( 1, button );
    }

    return button;
}

void ChapterSelectUI::onLeftNavigationClicked() {
    _carrousel->scrollRight();
}

void ChapterSelectUI::onRightNavigationClicked() {
    _carrousel->scrollLeft();
}

void ChapterSelectUI::updateNavigationButtons() {
    _left->setEnabled( !_carrousel->isBeginning() );
    _right->setEnabled( !_carrousel->isEnd() );
    update();
}

void ChapterSelectUI::carrouselItemClickedSlot( int time ) {
    emit carrouselItemClicked( time );
}

void ChapterSelectUI::itemsRepositionedSlot() {
    updateNavigationButtons();
}

void ChapterSelectUI::setItems( QList<IChapter*> items, int selectedChapterIndex ) {
    _carrousel->setItems( items, selectedChapterIndex );
    updateNavigationButtons();
}

void ChapterSelectUI::onVisibilityChange( bool visible ) {
    if( visible )
        _carrousel->update();
}

void ChapterSelectUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void ChapterSelectUI::changeItemChapterImage( int indexOfItem, QImage chapterImage, bool immediateUpdate ) {
    _carrousel->changeItemChapterImage( indexOfItem, chapterImage, immediateUpdate );
}

void ChapterSelectUI::setCurrentVideoLengthInSeconds( int seconds ) {
    _carrousel->setCurrentVideoLengthInSeconds( seconds );
}

