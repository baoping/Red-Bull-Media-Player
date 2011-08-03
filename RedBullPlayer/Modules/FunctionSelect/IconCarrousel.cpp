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
#include "IconCarrousel.h"

// QT includes
#include <QDebug>
#include <QWidget>

// Animation
#include "../../../3rdParty/src/qtanimationframework-2.3/src/qtpropertyanimation.h"

// Project Includes
#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;


IconCarrousel::IconCarrousel( QWidget *parentWidget ) :
    QObject( parentWidget ) {
    _parentWidget = parentWidget;
    _currentIndex = -1;
    ACTIVE_ITEM_SIZE = QSize( 120, 60 );
    ACTIVE_ITEM_Y_POS = 0;
    NON_ACTIVE_ITEM_Y_POS = 17;
    NON_ACTIVE_ITEM_DISTANCE = 175;
    NON_ACTIVE_ITEM_POS1 = QPoint( 160, 17 );
    NON_ACTIVE_ITEM_POS2 = QPoint( 510, 17 );
    EASING_CURVE = QtEasingCurve::InOutCubic;
    _animationOn = false;
    initItems();
    displayItems();
}


void IconCarrousel::scrollLeft() {
    if( _animationOn )
        return;

    _animationOn = true;
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );

    int previousWidgetIndex = getPreviousIndex();
    MirrorWidget *previousWidget = _items.at( previousWidgetIndex );
    addWidgetAnimation( previousWidget, group, previousWidget->scaleFactor(), previousWidget->pos(), 0, QPoint( _parentWidget->size().width() / 2, 10 ) );

    MirrorWidget *activeWidget = _items.at( _currentIndex );
    QPoint nonActivePos1 = getNonActiveItemPos( activeWidget, true );
    addWidgetAnimation( activeWidget, group, activeWidget->scaleFactor(), activeWidget->pos(), 0.4, QPoint( nonActivePos1.x(), nonActivePos1.y() ) );

    int nextWidgetIndex = getNextIndex();

    MirrorWidget *nextWidget = _items.at( nextWidgetIndex );
    QPoint activePos = getActiveItemPos( nextWidget );
    addWidgetAnimation( nextWidget, group, nextWidget->scaleFactor(), nextWidget->pos(), 1, QPoint( activePos.x(), activePos.y() ) );

    int newWidgetIndex = getNextVisibleItemIndexRight();

    MirrorWidget *newWidget = _items.at( newWidgetIndex );
    newWidget->show();

    QPoint nonActivePos2 = getNonActiveItemPos( newWidget, false );
    addWidgetAnimation( newWidget, group, 0, newWidget->pos(), 0.4, QPoint( nonActivePos2.x(), nonActivePos2.y() ) );
    group->start();
    _currentIndex++;

    if( _currentIndex >= _items.count() ) {
        _currentIndex -= _items.count();
    }
}


void IconCarrousel::scrollRight() {
    if( _animationOn )
        return;

    _animationOn = true;

    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    int previousWidgetIndex = getPreviousIndex();

    MirrorWidget *previousWidget = _items.at( previousWidgetIndex );
    QPoint activePos = getActiveItemPos( previousWidget );
    addWidgetAnimation( previousWidget, group, previousWidget->scaleFactor(), previousWidget->pos(), 1, QPoint( activePos.x(), activePos.y() ) );

    MirrorWidget *activeWidget = _items.at( _currentIndex );
    QPoint nonActivePos2 = getNonActiveItemPos( activeWidget, false );
    addWidgetAnimation( activeWidget, group, activeWidget->scaleFactor(), activeWidget->pos(), 0.4, QPoint( nonActivePos2.x(), nonActivePos2.y() ) );

    int nextWidgetIndex = getNextIndex();

    MirrorWidget *nextWidget = _items.at( nextWidgetIndex );
    addWidgetAnimation( nextWidget, group, nextWidget->scaleFactor(), nextWidget->pos(), 0, QPoint( _parentWidget->size().width() / 2, 10 ) );

    int newWidgetIndex = getNextVisibleItemIndexLeft();

    MirrorWidget *newWidget = _items.at( newWidgetIndex );
    newWidget->show();

    QPoint nonActivePos1 = getNonActiveItemPos( newWidget, true );
    addWidgetAnimation( newWidget, group, 0, newWidget->pos(), 0.4, QPoint( nonActivePos1.x(), nonActivePos1.y() ) );
    group->start();
    _currentIndex--;

    if( _currentIndex < 0 ) {
        _currentIndex += _items.count();
    }
}

int IconCarrousel::getNextVisibleItemIndexLeft() {
    int nextIndex = 0;
    nextIndex = _currentIndex - 2;

    if( nextIndex < 0 )
        nextIndex += _items.count();

    return nextIndex;
}

int IconCarrousel::getNextVisibleItemIndexRight() {
    int nextIndex = 0;
    nextIndex = _currentIndex + 2;

    if( nextIndex >= _items.count() )
        nextIndex -= _items.count();

    return nextIndex;
}

int IconCarrousel::getCurrentIndex() {
    return _currentIndex;
}

MirrorWidget *IconCarrousel::getCurrentWidget() {
    int index = getCurrentIndex();

    if( index >= 0 && index < _items.count() ) {
        return _items.at( getCurrentIndex() );
    } else {
        return NULL;
    }
}

void IconCarrousel::animationFinishedSlot() {
    int currentIndex = getCurrentIndex();
    int previous = getPreviousIndex();
    int next = getNextIndex();

    for( int i = 0; i < _items.count(); i++ ) {
        if( i != currentIndex && i != previous && i != next ) {
            _items.at( i )->hide();
        }
    }

    _animationOn = false;
    emit animationFinished();
}

void IconCarrousel::selectIconInTheMiddle() {
    iconClickedSlot( _currentIndex );
}

void IconCarrousel::iconClickedSlot( int id ) {
    QString title = _items.at( id )->title();
    int previousWidgetIndex = getPreviousIndex();

    if( _items.at( previousWidgetIndex )->title() == title ) {
        scrollRight();
    }

    int nextWidgetIndex = getNextIndex();

    if( _items.at( nextWidgetIndex )->title() == title ) {
        scrollLeft();
    }

    emit iconClicked( title );

    for( int i = 0; i < _items.count(); i++ ) {
        if( i != id ) {
            _items.at( i )->setActive( false );
        } else {
            _items.at( i )->setActive( true );
        }
    }
}

void IconCarrousel::initItems() {
    int i = 0;
    _items.clear();
    appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_TV_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_TV_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_TV_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_TV_URL,
                  Constants::FUNCTION_SELECT_TV_TITLE, true, i );
    i++;
    appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_MEDIA_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_MEDIA_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_MEDIA_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_MEDIA_URL,
                  Constants::FUNCTION_SELECT_MEDIA_TITLE, true, i );
    i++;

    if( ConfigManager::instance()->displayServusTvB2bButton() ) {
        appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_SERVUSTVB2B_URL,
                      Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_SERVUSTVB2B_URL,
                      Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_SERVUSTVB2B_URL,
                      Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_SERVUSTVB2B_URL,
                      Constants::FUNCTION_SELECT_SERVUSTVB2B_TITLE, true, i );
        i++;
    }

    appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_SETTINGS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_SETTINGS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_SETTINGS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_SETTINGS_URL,
                  Constants::FUNCTION_SELECT_SETTINGS_TITLE, true, i );
    i++;
    appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_NEWS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_NEWS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_NEWS_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_NEWS_URL,
                  Constants::FUNCTION_SELECT_NEWS_TITLE, true, i );
    i++;
    appendWidget( Constants::FUNCTION_SELECT_IMAGE_DIR + Constants::FUNCTION_SELECT_ABOUT_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "active" + Constants::FUNCTION_SELECT_ABOUT_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "dark" + Constants::FUNCTION_SELECT_ABOUT_URL,
                  Constants::FUNCTION_SELECT_IMAGE_DIR + "darkactive" + Constants::FUNCTION_SELECT_ABOUT_URL,
                  Constants::FUNCTION_SELECT_ABOUT_TITLE, true, i );
    i++;

    if( _items.count() > 0 ) {
        _currentIndex = 1;
    } else {
        _currentIndex = 0;
    }
}

void IconCarrousel::displayItems() {
    if( _items.count() == 0 || _currentIndex < 0 ) {
        return;
    }

    MirrorWidget *activeWidget = _items.at( _currentIndex );
    activeWidget->setActive( true );
    QPoint activePos = getActiveItemPos( activeWidget );
    activeWidget->move( activePos );
    activeWidget->show();

    if( _currentIndex - 1 >= 0 ) {
        MirrorWidget *previousWidget = _items.at( _currentIndex - 1 );
        previousWidget->setScaleFactor( 0.4 );
        QPoint nonActivePos1 = getNonActiveItemPos( previousWidget, true );
        previousWidget->move( nonActivePos1 );
        previousWidget->show();
    }

    if( _currentIndex + 1 < _items.count() ) {
        MirrorWidget *nextWidget = _items.at( _currentIndex + 1 );
        nextWidget->setScaleFactor( 0.4 );
        QPoint nonActivePos2 = getNonActiveItemPos( nextWidget, false );
        nextWidget->move( nonActivePos2 );
        nextWidget->show();
    }
}

void IconCarrousel::addWidgetAnimation( QWidget *w, QtParallelAnimationGroup *group, qreal startScaleFactor, QPoint startPos,
                                        qreal endScaleFactor, QPoint endPos ) {
    QtPropertyAnimation *a1 = new QtPropertyAnimation( w, "scaleFactor", _parentWidget );
    a1->setDuration( Constants::FUNCTION_SELECT_ANIMATION_LENGTH );
    a1->setEasingCurve( EASING_CURVE );
    a1->setStartValue( startScaleFactor );
    a1->setEndValue( endScaleFactor );
    group->addAnimation( a1 );

    QtPropertyAnimation *a2 = new QtPropertyAnimation( w, "pos", _parentWidget );
    a2->setDuration( Constants::FUNCTION_SELECT_ANIMATION_LENGTH );
    a2->setEasingCurve( EASING_CURVE );
    a2->setStartValue( startPos );
    a2->setEndValue( endPos );
    group->addAnimation( a2 );
}

void IconCarrousel::appendWidget( QString url, QString activeUrl, QString darkImgUrl,
                                  QString darkActiveImgUrl, QString title, bool hideOnStart, int id ) {
    MirrorWidget *newItem = new MirrorWidget( _parentWidget, url, activeUrl, darkImgUrl, darkActiveImgUrl, title, hideOnStart, id );
    _items.append( newItem );
    connect( newItem, SIGNAL( clicked( int ) ), this, SLOT( iconClickedSlot( int ) ) );
    newItem->scaleToSize( ACTIVE_ITEM_SIZE );
    newItem->move( _parentWidget->size().width() / 2, 10 );
}

QPoint IconCarrousel::getActiveItemPos( MirrorWidget *activeWidget ) {
    int x = ( _parentWidget->width() - activeWidget->width() ) / 2;
    int y = ACTIVE_ITEM_Y_POS;
    return QPoint( x, y );
}

QPoint IconCarrousel::getNonActiveItemPos( MirrorWidget *widget, bool left ) {
    int index = -1;

    if( !left ) {
        index = 1;
    }

    int x = (int)( _parentWidget->width() / 2 + NON_ACTIVE_ITEM_DISTANCE * index - 0.4 * widget->width() / 2 );
    int y = NON_ACTIVE_ITEM_Y_POS;
    return QPoint( x, y );
}

int IconCarrousel::getPreviousIndex() {
    int previousWidgetIndex = _currentIndex - 1;

    if( previousWidgetIndex < 0 ) {
        previousWidgetIndex += _items.count();
    }

    return previousWidgetIndex;
}

int IconCarrousel::getNextIndex() {
    int nextWidgetIndex = _currentIndex + 1;

    if( nextWidgetIndex >= _items.count() ) {
        nextWidgetIndex -= _items.count();
    }

    return nextWidgetIndex;
}

void IconCarrousel::addWidget( QString url, QString activeUrl, QString darkUrl, QString darkActiveUrl, QString title ) {
    appendWidget( url, activeUrl, darkUrl, darkActiveUrl, title, true, _items.count() );
    displayItems();
}

void IconCarrousel::changeFocusStatus( bool hasFocus ) {
    for( int i = 0; i < _items.count(); i++ )
        _items.at( i )->setMainRegionFocus( hasFocus );
}

void IconCarrousel::changeInternetConnectionStatus( bool isConnected ) {
    for( int i = 0; i < _items.count(); i++ ) {
        if( _items.at( i )->title() == Constants::FUNCTION_SELECT_TV_TITLE ) {
            _items.at( i )->setEnabled( isConnected );
        }
    }
}
