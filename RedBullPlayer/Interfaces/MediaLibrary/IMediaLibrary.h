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
#ifndef IMEDIALIBRARY_H
#define IMEDIALIBRARY_H


#include "../../Container/Plugin.h"

#include "IMediaDB.h"
#include "IMedia.h"
#include "IMediaMetadataHandler.h"
#include "ISearchEngine.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IMediaLibrary : public RedBullPlayer::Container::Plugin {
                    Q_OBJECT
                public:
                    virtual IMediaDB* mediaDB() const = 0;
                    virtual IMediaMetadataHandler* metadataHandler() const = 0;
                    virtual ISearchEngine* searchEngine() const = 0;
            };
        }
    }
}

Q_DECLARE_INTERFACE( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary, "at.mii.redbullplayer.modules.medialibrarymodule/1.0" )



#endif // IMEDIALIBRARY_H
