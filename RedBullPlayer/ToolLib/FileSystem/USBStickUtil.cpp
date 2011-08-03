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
#include <qglobal.h>
#include "USBStickUtil.h"

// Project
#include "../ConfigManager.h"

#if defined(Q_OS_WIN)
#include "Win32VolumeFinder.h"
#elif defined(Q_OS_MAC)
#else
#include "LinuxVolumeFinder.h"
#endif

using namespace RedBullPlayer::Tools;

USBStickUtil::USBStickUtil() {}

QString USBStickUtil::getRootPathForVolumeLabel( QString label ) {
#if defined(Q_OS_WIN)
    Win32VolumeFinder vf;
    return vf.findDriveByLabel( label );
#elif defined(Q_OS_MAC)
#else
    LinuxVolumeFinder vf;
    return vf.findDriveByLabel( label );
#endif
    return QString::null;
}

QString USBStickUtil::getStickRootPath() {
    return getRootPathForVolumeLabel( ConfigManager::instance()->getUSBStickName() );
}


QString USBStickUtil::getVolumeLabelForPath( QString path ) {
#if defined(Q_OS_WIN)
    Win32VolumeFinder vf;
    return vf.getVolumeLabelForPath( path );
#elif defined(Q_OS_MAC)
#else
    LinuxVolumeFinder vf;
    return vf.getVolumeLabelForPath( path );
#endif
    return QString::null;
}
