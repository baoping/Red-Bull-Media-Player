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
#include "VLCInstance.h"

#include "VLCException.h"

VLCInstance::VLCInstance( MediaPlayerPurpose::Purpose purpose ) {
    if ( purpose == MediaPlayerPurpose::PLAY_BACK ) {
#if defined(Q_OS_MAC)
        const char * const vlc_args[] = {
            //"--verbose=0",
            "--vout", "macosx",
            "--plugin-path=plugins",
            "--no-plugins-cache",
            "--no-video-title-show",
            "--no-media-library",
            "--no-keyboard-events",
            "--no-mouse-events",
            "--no-stats",
            "--file-caching=3000",
            "--mouse-hide-timeout=5000"
        };
        _vlcInstance = libvlc_new( sizeof( vlc_args ) / sizeof( vlc_args[0] ), vlc_args );
        libvlc_set_log_verbosity( _vlcInstance, 0 );
#else
        const char * const vlc_args[] = {
            //"--verbose=0",
            "--plugin-path=plugins",
            "--no-plugins-cache",
            "--no-video-title-show",
            "--no-media-library",
            "--no-keyboard-events",
            "--no-mouse-events",
            "--no-stats",
            "--file-caching=3000",
            "--mouse-hide-timeout=5000"
        };
        _vlcInstance = libvlc_new( sizeof( vlc_args ) / sizeof( vlc_args[0] ), vlc_args );
        libvlc_set_log_verbosity( _vlcInstance, 0 );
#endif
    } else if ( purpose == MediaPlayerPurpose::SNAPSHOT_CREATION ) {
        const char* const vlc_args[] = {
            "--vout", "dummy",
            "--no-keyboard-events",
            "--no-mouse-events",
            "--no-audio",
            "--no-media-library",
            "--no-video-title-show",
            "--no-stats",
            "--plugin-path=plugins",
            "--no-plugins-cache"
        };
        _vlcInstance = libvlc_new( sizeof( vlc_args ) / sizeof( vlc_args[0] ), vlc_args );
        libvlc_set_log_verbosity( _vlcInstance, 0 );
    }

    if ( !_vlcInstance )
        throw new VLCException( libvlc_errmsg() );
}

VLCInstance::~VLCInstance() {
    libvlc_release( _vlcInstance );
}
