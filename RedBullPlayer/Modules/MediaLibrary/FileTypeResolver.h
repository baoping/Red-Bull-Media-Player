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
#ifndef FILETYPERESOLVER_H
#define FILETYPERESOLVER_H

// TagLib
#include <tag.h>
#include <fileref.h>
#include <tfile.h>
#include <tstring.h>
#include <mpegfile.h>
#include <id3v2header.h>
#include <id3v2frame.h>
#include <attachedpictureframe.h>
#include <id3v2tag.h>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            class FileTypeResolver : public TagLib::FileRef::FileTypeResolver {
                public:
                    virtual TagLib::File *createFile( TagLib::FileName fileName, bool readAudioProperties = true, TagLib::AudioProperties::ReadStyle = TagLib::AudioProperties::Average ) const;

                private:
                    bool hasID3Signature( TagLib::FileName fileName ) const;

            };
        }
    }
}
#endif // FILETYPERESOLVER_H
