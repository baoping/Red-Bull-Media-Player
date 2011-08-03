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
#include "ChapterCarrousel.h"

// Qt Includes
#include <QDebug>
#include <QLabel>

// Animation
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qtpropertyanimation.h"

// Project Includes
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;

ChapterCarrousel::ChapterCarrousel( QWidget *parent ) : QObject( parent ) {
    _parentWidget = parent;
    _currentIndex = -1;
    _firstIndexOnScreen = -1;
    _currentVideoLengthSeconds = 0;
    _userSelectedChapter = false;
    _animationOn = false;
    Aggregator::instance()->registerListener( PlayerTimeChanged, this );
}

void ChapterCarrousel::scrollRight() {
    if( _animationOn )
        return;

    _animationOn = true;
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    int distance = Constants::CHAPTER_SELECT_ITEMS_COUNT * getItemWidth();
    int count = Constants::CHAPTER_SELECT_ITEMS_COUNT;

    if( _firstIndexOnScreen - Constants::CHAPTER_SELECT_ITEMS_COUNT < 0 ) {
        distance = _firstIndexOnScreen * getItemWidth();
        count = _firstIndexOnScreen;
    }

    for( int i = _firstIndexOnScreen; i < _firstIndexOnScreen + Constants::CHAPTER_SELECT_ITEMS_COUNT; i++ ) {
        if( i >= 0 && i < _widgets.count() ) {
            addWidgetAnimation( _widgets.at( i ), group, _widgets.at( i )->pos(), QPoint( distance + _widgets.at( i )->pos().x(),
                                _widgets.at( i )->pos().y() ) );
        }
    }

    for( int i = 0; i < count; i++ ) {
        int index = _firstIndexOnScreen - i - 1;

        if( index >= 0 && index < _widgets.count() ) {
            addWidgetAnimation( _widgets.at( index ), group, QPoint( -distance + getItemXPos( i ), _widgets.at( i )->pos().y() ),
                                QPoint( getItemXPos( i ), _widgets.at( i )->pos().y() ) );
        }
    }

    group->start();
    _firstIndexOnScreen = _firstIndexOnScreen - count;
    emit itemsRepositioned();
}

void ChapterCarrousel::scrollLeft() {
    if( _animationOn )
        return;

    _animationOn = true;
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    int distance = Constants::CHAPTER_SELECT_ITEMS_COUNT * getItemWidth();
    int count = Constants::CHAPTER_SELECT_ITEMS_COUNT;

    for( int i = _firstIndexOnScreen; i < _firstIndexOnScreen + Constants::CHAPTER_SELECT_ITEMS_COUNT; i++ ) {
        if( i >= 0 && i < _widgets.count() ) {
            addWidgetAnimation( _widgets.at( i ), group, _widgets.at( i )->pos(), QPoint( -distance + _widgets.at( i )->pos().x(),
                                _widgets.at( i )->pos().y() ) );
        }
    }

    for( int i = 0; i < count; i++ ) {
        int index = _firstIndexOnScreen + count + i;

        if( index >= 0 && index < _widgets.count() ) {
            addWidgetAnimation( _widgets.at( index ), group, QPoint( distance + getItemXPos( i ), _widgets.at( i )->pos().y() ),
                                QPoint( getItemXPos( i ), _widgets.at( i )->pos().y() ) );
        }
    }

    group->start();
    _firstIndexOnScreen = _firstIndexOnScreen + count;
    emit itemsRepositioned();
}

void ChapterCarrousel::setItems( QList<IChapter*> items, int selectedChaperIndex ) {
    clearItems();
    _items = items;
    initItems();

    if( selectedChaperIndex >= 0 )
        selectChapterItem( selectedChaperIndex );

    displayItems();
}

bool ChapterCarrousel::isBeginning() {
    if( _widgets.count() == 0 || _firstIndexOnScreen == 0 ) {
        return true;
    }

    return false;
}


bool ChapterCarrousel::isEnd() {
    if( _widgets.count() == 0 || _firstIndexOnScreen >= _widgets.count() - Constants::CHAPTER_SELECT_ITEMS_COUNT ) {
        return true;
    }

    return false;
}

void ChapterCarrousel::update( bool displayAccordingToCurrIndex/* = true*/ ) {
    if( _firstIndexOnScreen >= 0 ) {
        displayItems( displayAccordingToCurrIndex );
    }
}

void ChapterCarrousel::itemClickedSlot( int id ) {
    if( id >= 0 && id < _items.length() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, Q_FUNC_INFO );
        SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, "begin" );
        _userSelectedChapter = true;
        int time = _items.at( id )->timeInMS();
        emit itemClicked( time );
        SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, "end" );
    }
}

void ChapterCarrousel::initItems() {
    Q_ASSERT( _widgets.count() == 0 );
    int itemHeight = getItemHeight();
    int itemWidth = getItemWidth();

    for( int i = 0; i < _items.count(); i++ ) {
        CarrouselItem *img = new CarrouselItem( _items.at( i )->snapshot(), itemWidth, itemHeight, _parentWidget,
                                                ( qint64 )( _items.at( i )->timeInMS() / 1000 ), _currentVideoLengthSeconds, i );
        connect( img, SIGNAL( clicked( int ) ), this, SLOT( itemClickedSlot( int ) ) );
        _widgets.append( img );
    }

    _currentIndex = 0;
    _firstIndexOnScreen = 0;
}

void ChapterCarrousel::displayItems( bool displayAccordingToCurrIndex/* = true*/ ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, "begin" );
    SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, QString( "displayAccordingToCurrIndex = %1" ).arg( displayAccordingToCurrIndex ) );

    for( int i = 0; i < _widgets.count(); i++ ) {
        _widgets.at( i )->hide();
    }

    if( displayAccordingToCurrIndex ) {
        _firstIndexOnScreen = _currentIndex;
        emit itemsRepositioned();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_CHAPTERSELECT, QString( "_firstIndexOnScreen = %1" ).arg( _firstIndexOnScreen ) );

    for( int i = 0; i < Constants::CHAPTER_SELECT_ITEMS_COUNT; i++ ) {
        int index = _firstIndexOnScreen + i;

        if( index >= 0 && index < _widgets.count() ) {
            displayItem( _widgets.at( index ), i );
        }
    }

    if( _widgets.length() > 0 && _currentIndex == 0 ) {
        _widgets.at( 0 )->setHighlighted( true );
    }
}

void ChapterCarrousel::displayItem( CarrouselItem* widget, int indexOnScreen ) {
    widget->move( getItemXPos( indexOnScreen ), getItemYPos( indexOnScreen ) );
    widget->show();
}

int ChapterCarrousel::getItemXPos( int indexOnScreen ) {
    return indexOnScreen * getItemWidth();
}

int ChapterCarrousel::getItemYPos( int /*indexOnScreen*/ ) {
    return 0;
}

int ChapterCarrousel::getItemWidth() {
    return _parentWidget->width() / Constants::CHAPTER_SELECT_ITEMS_COUNT;
}

int ChapterCarrousel::getItemHeight() {
    return _parentWidget->height();
}

void ChapterCarrousel::clearItems() {
    for( int i = _items.count() - 1; i >= 0; i-- ) {
        delete _items.at( i );
    }

    _items.clear();

    for( int i = _widgets.count() - 1; i >= 0; i-- ) {
        _widgets.at( i )->deleteLater();
    }

    _widgets.clear();
    _currentIndex = -1;
    _firstIndexOnScreen = -1;
    _userSelectedChapter = false;
}

void ChapterCarrousel::sort( QList<IChapter*> *_items ) {
    bool wasChange = true;

    while( wasChange ) {
        wasChange = false;

        for( int i = 0; i < _items->count() - 1; i++ ) {
            if( _items->at( i )->timeInMS() > _items->at( i + 1 )->timeInMS() ) {
                _items->swap( i, i + 1 );
                wasChange = true;
            }
        }
    }
}

void ChapterCarrousel::addWidgetAnimation( QWidget *w, QtParallelAnimationGroup *group, QPoint startPos, QPoint endPos ) {
    QtPropertyAnimation *a = new QtPropertyAnimation( w, "pos", _parentWidget );
    int distance = abs( endPos.x() - startPos.x() );
    a->setDuration( (int)( distance / Constants::CHAPTER_SELECT_ANIMATION_SPEED ) );
    a->setEasingCurve( QtEasingCurve::Linear );
    a->setStartValue( startPos );
    a->setEndValue( endPos );
    w->show();
    group->addAnimation( a );
}

void ChapterCarrousel::animationFinishedSlot() {
    update( false );
    _animationOn = false;
}

bool ChapterCarrousel::event ( QEvent * e ) {
    if( e->type() == PlayerTimeChanged ) {
        onPlayerTimeChanged( static_cast<PlayerTimeChangedEvent*>( e ) );
    }

    return false;
}

void ChapterCarrousel::onPlayerTimeChanged( PlayerTimeChangedEvent* e ) {
    if( _widgets.length() == 0 || _currentIndex < 0 || _currentIndex >= _widgets.length() ) return;

    int time = e->time();
    int nearestLowerIndex = 0;

    for( int i = _items.length() - 1; i >= 0; i-- ) {
        if( _items.at( i )->timeInMS() <= time ) {
            nearestLowerIndex = i;
            break;
        }
    }

    if( _currentIndex != nearestLowerIndex ) {
        selectChapterItem( nearestLowerIndex );
        displayItems( true );
    }
}

void ChapterCarrousel::changeItemChapterImage( int indexOfItem, QImage chapterImage, bool immediateUpdate ) {
    _widgets.at( indexOfItem )->changeChapterImage( chapterImage, immediateUpdate );
}

void ChapterCarrousel::setCurrentVideoLengthInSeconds( int currentVideoLengthInSeconds ) {
    _currentVideoLengthSeconds = currentVideoLengthInSeconds;
}

int ChapterCarrousel::getItemCount() {
    return _items.count();
}

void ChapterCarrousel::selectChapterItem( int chapterIndex ) {
    if( _widgets.count() == 0 )
        return;

    _currentIndex = chapterIndex;

    for( int i = 0; i < _widgets.count(); i++ ) {
        if( _widgets.at( i )->highLighted() ) {
            _widgets.at( i )->setHighlighted( false );
            break;
        }
    }

    _widgets.at( _currentIndex )->setHighlighted( true );
}
