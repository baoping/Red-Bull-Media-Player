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
#ifndef CHAPTER_H
#define CHAPTER_H

#include "../../Interfaces/IChapter.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {
            class Chapter : public RedBullPlayer::Modules::ChapterSelect::IChapter {
                public:
                    Chapter();

                    virtual QUuid id() const;

                    virtual QImage snapshot();
                    virtual void setSnapshot( QImage img );

                    virtual int timeInMS() const;
                    virtual void setTimeInMS( int ms );

                    virtual float position() const;
                    virtual void setPosition( float position );

                private:
                    QImage _image;
                    int _ms;
                    QUuid _id;
                    float _position;
            };
        }
    }
}

#endif // CHAPTER_H
