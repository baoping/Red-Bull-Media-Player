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
#include "RssFeedParser.h"

#include "../../ToolLib/helper.h"
#include "../../ToolLib/RssFeed/RssFeed.h"

using namespace RedBullPlayer::Modules::XmlNewsFeed;
using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Tools::Network;

RssFeedParser::RssFeedParser( QObject *parent ) : QObject( parent ) {
    _isRunning = false;
    _nam = new NetworkAccessManager( this );
    connect( _nam, SIGNAL( finished( QUrl, QByteArray, QString, QString ) ), this, SLOT( getFinished( QUrl, QByteArray, QString, QString ) ) );
    connect( _nam, SIGNAL( failed( QUrl, int, QString ) ), this, SLOT( getFailed( QUrl, int, QString ) ) );
}

RssFeedParser::~RssFeedParser() {
    _nam->deleteLater();
}

void RssFeedParser::parseRssFeed( QString rssFeedUrl ) {
    _isRunning = true;
    _nam->get( Helper::decodeUrl( rssFeedUrl ) );
}

void RssFeedParser::getFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength ) {
    IRssFeed* rssFeed = RssFeed::parseFeed( data );

    if ( rssFeed != 0 ) {
        emit parseSuccessful( rssFeed );
    } else {
        emit parseFailed();
    }

    _isRunning = false;
}

void RssFeedParser::getFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase ) {
    emit parseFailed();
    _isRunning = false;
}
