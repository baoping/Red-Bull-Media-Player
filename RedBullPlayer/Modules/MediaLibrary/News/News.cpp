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
#include "News.h"

using namespace RedBullPlayer::Modules::MediaLibrary;

News::News( const QUuid& guid, const QString& title ) {
    _guid = guid;
    _title = title;
}

News::News( const INews& news ) {
    _guid = news.guid();
    _title = news.title();
    _link = news.link();
    _description = news.description();
    _content = news.content();
    _author = news.author();
    _category = news.category();
    _externalGuid = news.externalGuid();
    _pubDate = news.pubDate();
}

News::~News() {}

bool News::isValid() const {
    if ( title().isNull() || title().simplified().isEmpty() ) {
        return false;
    }

    return true;
}
