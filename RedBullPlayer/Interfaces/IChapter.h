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
#ifndef ICHAPTER_H
#define ICHAPTER_H

//Qt Includes
#include <QString>
#include <QImage>
#include <QUuid>

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {
            class IChapter {
                public:
                    virtual ~IChapter() {}

                    virtual QUuid id() const = 0;

                    virtual QImage snapshot() = 0;
                    virtual void setSnapshot( QImage img ) = 0;

                    virtual int timeInMS() const = 0;
                    virtual void setTimeInMS( int ms ) = 0;

                    virtual float position() const = 0;
                    virtual void setPosition( float position ) = 0;
            };
        }
    }
}

#endif // ICHAPTER_H
