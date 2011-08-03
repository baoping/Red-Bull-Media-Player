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
#ifndef SNAPSHOT_H
#define SNAPSHOT_H
// Qt includes
#include <QUuid>
#include <QString>
#include <QImage>

// Projectincludes
#include "MediaDB.h"
#include "../../Interfaces/MediaLibrary/ISnapshot.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class Snapshot : public ISnapshot {
                public:
                    Snapshot( QUuid guid, QImage image, float position, IMedia* media );
                    Snapshot( const ISnapshot& snapshot );
                    virtual ~Snapshot();
                    virtual QUuid guid() const {
                        return _guid;
                    }

                    virtual QImage image() const {
                        return _image;
                    }

                    virtual float position() const {
                        return _postion;
                    }

                    virtual IMedia* media() const {
                        return _media;
                    }

                private: // DATA
                    QUuid _guid;
                    QImage _image;
                    float _postion;
                    IMedia* _media;
            };
        }
    }
}
#endif // SNAPSHOT_H
