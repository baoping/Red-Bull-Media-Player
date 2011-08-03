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
#include "PodCastPoller.h"

#include <QDebug>

#include "../../Events/RssFeed/RssFeedParseRequestEvent.h"
#include "../../Events/RssFeed/RssFeedParseResponseEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::RSS;

PodCastPoller::PodCastPoller( QObject *parent ) :
    QObject( parent ) {
    _requestId  = UuidCreator::create();
    _pollTimer = new QTimer( this );
    connect( _pollTimer, SIGNAL( timeout() ), this, SLOT( pollRssFeed() ) );
    Aggregator::instance()->registerListener( RssFeedParseResponse, this );
}

PodCastPoller::~PodCastPoller() {
    Aggregator::instance()->unregisterListener( this );

    if ( _pollTimer->isActive() )
        _pollTimer->stop();

    _pollTimer->deleteLater();
}

bool PodCastPoller::isPolling() {
    return _pollTimer->isActive();
}

bool PodCastPoller::canStart() {
    return ConfigManager::instance()->podCastAvailable();
}

void PodCastPoller::startPoll( int interval ) {
    stopPoll();

    if ( canStart() ) {
        qDebug() << "PodCastPoller => StartPoll(Interval=" << interval << ")";
        _pollTimer->setInterval( interval );
        pollRssFeed();
    }
}

void PodCastPoller::stopPoll() {
    if ( _pollTimer->isActive() )
        _pollTimer->stop();
}

void PodCastPoller::pollRssFeed() {
    QString podCastUrl = ConfigManager::instance()->getPodCastUrl().toString();
    qDebug() << "PodCastPoller => SendParseRequest";
    RssFeedParseRequestEvent ev( _requestId, podCastUrl );
    Aggregator::instance()->sendEvent( &ev );
    _pollTimer->start();
}

bool PodCastPoller::event( QEvent* event ) {
    if ( event->type() == RssFeedParseResponse ) {
        RssFeedParseResponseEvent* response = static_cast<RssFeedParseResponseEvent*>( event );

        if ( response->requestId() == _requestId ) {
            qDebug() << "PodCastPoller => Emit RssFeedRetrieved";
            IRssFeed* feed = response->rssFeed();
            emit rssFeedRetrieved( feed );
        }
    }

    return false;
}

