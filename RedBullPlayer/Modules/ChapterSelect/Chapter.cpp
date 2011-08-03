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
#include "Chapter.h"

#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Tools;

Chapter::Chapter() {
    _id = UuidCreator::create();
}

QUuid Chapter::id() const {
    return _id;
}

QImage Chapter::snapshot() {
    return _image;
}

void Chapter::setSnapshot( QImage img ) {
    _image = img;
}

int Chapter::timeInMS() const {
    return _ms;
}

void Chapter::setTimeInMS( int ms ) {
    _ms = ms;
}

float Chapter::position() const {
    return _position;
}

void Chapter::setPosition( float position ) {
    _position = position;
}

