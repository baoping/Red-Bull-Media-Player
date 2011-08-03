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
#include "TagLibBasedMetaDataHandler.h"

// QT includes
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

// std includes
#include <iostream>
#include <stdio.h>
#include <ostream>
#include <fstream>
#include <string.h>

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

// Project includes
#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

#include "FileTypeResolver.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


TagLibBasedMetaDataHandler::TagLibBasedMetaDataHandler( IMediaLibrary* mediaLibrary, QObject* parent )
    : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    TagLib::FileRef::addFileTypeResolver( new RedBullPlayer::Modules::MediaLibrary::FileTypeResolver );
}

void TagLibBasedMetaDataHandler::setMetaDataIfAvailable( IMedia* media ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "At file: %1" ).arg( media->fileName() ) );
    TagLib::FileRef fileRef( media->fileName().toUtf8().data(), true, TagLib::AudioProperties::Accurate );
    TagLib::Tag* tag = fileRef.tag();

    if ( tag && ! tag->isEmpty() ) {
        if ( ! tag->title().isNull() ) {
            QFileInfo tagTitleName( TStringToQString( tag->title() ) );

            if ( ! tagTitleName.exists() ) {
                SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Setting tile tag: %1" ).arg( TStringToQString( tag->title() ) ) );
                media->setTitle( TStringToQString( tag->title() ) );
            }
        }

        if ( ! tag->album().isNull() ) {
            media->setAlbum( TStringToQString( tag->album() ) );
        }

        if ( ! tag->artist().isNull() ) {
            media->setArtist( TStringToQString( tag->artist() ) );
        }

        if ( ! tag->comment().isNull() ) {
            media->setDescription( TStringToQString( tag->comment() ) );
        }

        if ( ! tag->genre().isNull() ) {
            media->setGenre( TStringToQString( tag->genre() ) );
        }

        if ( tag->track() != 0 ) {
            media->setTrackNumber( tag->track() );
        }
    } else {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "tag == 0 or tag.isEmpty()" );
    }

    if( !fileRef.isNull() && fileRef.audioProperties() ) {
        TagLib::AudioProperties *properties = fileRef.audioProperties();
        media->setLength( properties->length() * 1000 ); //save as MS;
    } else {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "tag == 0 or tag.isEmpty()" );
    }

    QImage coverArt = getAlbumCoverFromMP3( media->fileName() );

    if ( ! coverArt.isNull() ) {
        media->setCoverImage( coverArt );
    }
}


QImage TagLibBasedMetaDataHandler::getAlbumCoverFromMP3( QString fileName ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QImage anyImageAsCover;
    QImage coreectlyTypedCoverImage;
    static const char *IdPicture = "APIC" ;

    TagLib::FileRef fileRef( fileName.toUtf8().data(), true, TagLib::AudioProperties::Accurate );
    TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>( fileRef.file() );

    if ( ! mp3File ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "TagLib: Could not cast to mpeg file" );
        return anyImageAsCover;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );

    if( mp3File && mp3File->ID3v2Tag() ) {
        TagLib::ID3v2::FrameList frameList = mp3File->ID3v2Tag()->frameListMap()[IdPicture] ;

        if ( !frameList.isEmpty() ) {
            for( TagLib::ID3v2::FrameList::ConstIterator it = frameList.begin(); it != frameList.end(); ++it ) {
                TagLib::ID3v2::AttachedPictureFrame * pictureFrame = ( TagLib::ID3v2::AttachedPictureFrame * )( *it ) ;
                SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, QString( "TagLib: Found Picture frame of type: %1" ).arg( pictureFrame->type() ) );
                size_t size = pictureFrame->picture().size();
                anyImageAsCover.loadFromData( ( const uchar* )pictureFrame->picture().data(), size );

                //anyImageAsCover.save( QString("%1_cover_%2.jpg").arg( fileName ).arg( pictureFrame->type() ) );
                if ( pictureFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover ) { // extract image (in it's compressed form)
                    size_t size = pictureFrame->picture().size();
                    coreectlyTypedCoverImage.loadFromData( ( const uchar* )pictureFrame->picture().data(), size );
                }
            }
        }
    } else {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "TagLib: No ID3V2 Tag found" );
    }

    if ( ! coreectlyTypedCoverImage.isNull() ) {
        return coreectlyTypedCoverImage;
    }

    return anyImageAsCover;
}
