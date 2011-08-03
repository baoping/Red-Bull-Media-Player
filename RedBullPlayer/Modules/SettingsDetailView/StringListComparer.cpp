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
#include "StringListComparer.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;

StringListComparer::StringListComparer() {}

QStringList StringListComparer::getRemovedItems( QStringList originalList, QStringList newList ) {
    QStringList removedItems;

    for( int i = 0; i < originalList.size(); i++ )
        if( !newList.contains( originalList.at( i ), Qt::CaseSensitive ) )
            removedItems.append( originalList.at( i ) );

    return removedItems;
}

QStringList StringListComparer::getAddedItems( QStringList originalList, QStringList newList ) {
    QStringList addedItems;

    for( int i = 0; i < newList.size(); i++ )
        if( !originalList.contains( newList.at( i ), Qt::CaseSensitive ) )
            addedItems.append( newList.at( i ) );

    return addedItems;
}
