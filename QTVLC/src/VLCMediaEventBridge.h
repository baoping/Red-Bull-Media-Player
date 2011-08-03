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
#ifndef VLCMEDIAEVENTBRIDGE_H
#define VLCMEDIAEVENTBRIDGE_H

// VLC Includes
#include "vlc/vlc.h"

// Project Forwards
class VLCMediaPlayer;

class VLCMediaEventBridge {

    public:
        VLCMediaEventBridge( VLCMediaPlayer* mediaPlayer, libvlc_media_t* media );
        virtual ~VLCMediaEventBridge();

    public:
        void attachEvents();
        void detachEvents();

    private:
        static void onVLCEvent( const libvlc_event_t *, void * );

        void attachEvent( libvlc_event_type_t event_type );
        void detachEvent( libvlc_event_type_t event_type );

    private: // Data
        VLCMediaPlayer* _mediaPlayer;
        libvlc_event_manager_t* _eventManager;
};

#endif // VLCMEDIAEVENTBRIDGE_H
