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
#include "Tag.h"

// Project Includes
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

Tag::Tag( QUuid guid, QString name, bool builtIn  ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( ! guid.isNull() );
    Q_ASSERT( ! name.isNull() );
    _guid = guid;
    _name = name;
    _builtIn = builtIn;
}

Tag::Tag( const ITag& tag ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    _guid = tag.guid();
    _name = tag.name();
    _builtIn = tag.builtIn();
}

void Tag::setName( QString name, Error* error ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( ! name.isNull() );

    if ( name == _name ) {
        return;
    }

    if ( name.isNull() ) {
        SimpleLogger::instance()->error( Constants::LOGGER_MEDIALIBRARY, "Tag name cannot be an empty string" );
        error->setErrorMessage( "Tag name cannot be an empty string" );
        return;
    }

    _name = name;
    _dirty = true;
}
