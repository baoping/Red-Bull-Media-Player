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
#ifndef TAG_H
#define TAG_H

// Qt includes
#include <QUuid>
#include <QString>

// Projectincludes
#include "MediaDB.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class Tag : public ITag {
                friend class MediaDB;

            public:
                Tag( QUuid guid, QString name, bool _builtIn = false );
                Tag( const ITag& tag );
                virtual QUuid guid() const {
                    return _guid;
                }

                virtual QString name() const {
                    return _name;
                }

                virtual bool builtIn() const {
                    return _builtIn;
                }

                virtual void setName( QString name, RedBullPlayer::Container::Error* error );

            private: // Methods
                Tag();
                void setClean() {
                    _dirty = false;
                }

                bool dirty() const {
                    return _dirty;
                }

            private: // Data
                QUuid _guid;
                QString _name;
                bool _builtIn;
                bool _dirty;
            };
        }
    }
}


#endif // TAG_H
