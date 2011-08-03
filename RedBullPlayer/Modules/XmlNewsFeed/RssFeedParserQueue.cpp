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
#include "RssFeedParserQueue.h"

using namespace RedBullPlayer::Modules::XmlNewsFeed;
using namespace RedBullPlayer::Tools::RSS;

RssFeedParserQueue::RssFeedParserQueue( QObject *parent ) : QObject( parent ) {
    _parser = new RssFeedParser( this );
    connect( _parser, SIGNAL( parseSuccessful( RedBullPlayer::Tools::RSS::IRssFeed* ) ), this, SLOT( parserSuccessful( RedBullPlayer::Tools::RSS::IRssFeed* ) ) );
    connect( _parser, SIGNAL( parseFailed() ), this, SLOT( parserFailed() ) );
}

RssFeedParserQueue::~RssFeedParserQueue() {
    _jobs.clear();
    _parser->deleteLater();
}

void RssFeedParserQueue::enqueueParse( QUuid parseId, QString url ) {
    QPair<QUuid, QString> parseInfo;
    parseInfo.first = parseId;
    parseInfo.second = url;
    _jobs.enqueue( parseInfo );

    if ( !_parser->isRunning() ) {
        doNextParse();
    }
}

void RssFeedParserQueue::doNextParse() {
    if ( _jobs.count() > 0 ) {
        QPair<QUuid, QString> parseInfo = _jobs.dequeue();
        _currentJob = parseInfo.first;
        _parser->parseRssFeed( parseInfo.second );
    }
}

void RssFeedParserQueue::parserSuccessful( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed ) {
    emit parseSuccessful( _currentJob, rssFeed );
    doNextParse();
}

void RssFeedParserQueue::parserFailed() {
    emit parseFailed( _currentJob );
    doNextParse();
}


