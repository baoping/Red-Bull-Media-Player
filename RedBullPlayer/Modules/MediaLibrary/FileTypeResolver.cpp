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
#include "FileTypeResolver.h"

// Used by TagLib related stuff

#include <sys/stat.h>

#ifdef _WIN32
# include <wchar.h>
# include <windows.h>
# include <io.h>
# define ftruncate _chsize
#else
# include <unistd.h>
#endif

#include <stdlib.h>

// Qt includes
#include <QString>
#include <QFile>
#include <QFileInfo>

// Project includes
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"


#include <tstring.h>

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

TagLib::File* RedBullPlayer::Modules::MediaLibrary::FileTypeResolver::createFile(
    TagLib::FileName fileName,
    bool readAudioProperties,
    TagLib::AudioProperties::ReadStyle audioPropertiesStyle ) const {
    if ( hasID3Signature( fileName ) ) {
        return new TagLib::MPEG::File( fileName, readAudioProperties, audioPropertiesStyle );
    }

    return 0;
}

const char* id3TagIdentifier = "ID3";
uint id3TagIdentifierLength = sizeof( id3TagIdentifier ) - 1;
bool FileTypeResolver::hasID3Signature( TagLib::FileName fileName ) const {
    QString file;
#ifdef _WIN32
    file = ( wcslen( ( const wchar_t * ) fileName ) > 0 ) ? TStringToQString(  TagLib::String( ( const wchar_t * ) fileName ) ) : TStringToQString(  TagLib::String( ( const char * ) fileName ) ) ;
#else
    file = fileName;
#endif
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Parsing file: %1" ).arg( file ) );
    QFile qfile( file );

    if ( ! qfile.open( QIODevice::ReadOnly ) ) {
        return false;
    }

    char currentChar;
    uint state = 0;

    while( qfile.getChar( &currentChar ) ) {
        if( currentChar == id3TagIdentifier[state] ) {
            state++;
        } else {
            state = 0;
        }

        if( state == id3TagIdentifierLength ) {
            break;
        }
    }

    qfile.close();

    if ( state == id3TagIdentifierLength ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "File: %1 has a ID3 Tag" ).arg( file ) );
        return true;
    }

    return false;
}
