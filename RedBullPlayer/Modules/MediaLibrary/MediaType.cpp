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
#include "MediaType.h"

//logging
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::MediaLibrary;

MediaType::MediaType( QUuid guid, QString name ) {
    Q_ASSERT( ! guid.isNull() );
    Q_ASSERT( ! name.isNull() );
    _guid = guid;
    _name = name;
}

MediaType::MediaType( const IMediaType& mediaType ) {
    _guid = mediaType.guid();
    _name = mediaType.name();
}
