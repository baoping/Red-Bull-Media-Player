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
#include "DummyChapter.h"

//Qt Includes
#include <QDebug>

using namespace RedBullPlayer::Modules::ChapterSelect;

DummyChapter::DummyChapter() {
}

QString DummyChapter::getSnapshotUrl() {
    return _url;
}

void DummyChapter::setSnapshotUrl( QString url ) {
    _url = url;
}

int DummyChapter::getTimeInMilliseconds() const {
    return _millies;
}

void DummyChapter::setTimeInMilliseconds( int millies ) {
    _millies = millies;
}
