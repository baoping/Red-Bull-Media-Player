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
#include "CocoaContainer.h"

// Cocoa includes
#include <Cocoa/Cocoa.h>
#include <QGridLayout>

// Qt Includes
#include <QMacCocoaViewContainer>

// Project Includes
#include "../../3rdParty/MacOSX/Frameworks/VLCKit.framework/Headers/VLCVideoView.h"

QWidget *CocoaContainer::createCocoaWidget(libvlc_media_player_t* mediaplayer, QWidget *_cocoaParent)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    VLCVideoView *videoView = [[VLCVideoView alloc] init];

    QMacCocoaViewContainer *cocoa = new QMacCocoaViewContainer(NULL, _cocoaParent);    

    cocoa->setCocoaView(videoView);
    libvlc_media_player_set_nsobject ( mediaplayer, videoView );

    [videoView release];
    [pool release];

    return cocoa;
}
