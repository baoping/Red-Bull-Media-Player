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
#ifndef DUMMYCHAPTER_H
#define DUMMYCHAPTER_H

#include "../../Interfaces/IChapter.h"

class DummyChapter : public RedBullPlayer::Modules::ChapterSelect::IChapter {
    public:
        DummyChapter();

        virtual QString getSnapshotUrl();
        virtual void setSnapshotUrl( QString url );
        virtual int getTimeInMilliseconds() const;
        virtual void setTimeInMilliseconds( int millies );

    private:
        QString _url;
        int _millies;
};

#endif // DUMMYCHAPTER_H
