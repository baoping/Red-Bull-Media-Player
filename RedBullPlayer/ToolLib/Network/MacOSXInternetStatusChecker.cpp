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
#include "InternetStatusChecker.h"
#include "InternetStatusChecker.h"

// QT includes
#include <QDebug>

#include <assert.h>
#include <errno.h>

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

using namespace RedBullPlayer::Tools::Network;

InternetStatusChecker::~InternetStatusChecker() {}

bool InternetStatusChecker::internetAvailable() {
    qDebug() << Q_FUNC_INFO;
    int err = noErr;
    bool ok = false;
    SCNetworkReachabilityRef thisTarget;
    std::string str = std::string( _macOSXreferenceHost.toAscii().data() );
    const char* name = str.c_str();
    qDebug() << "checking host: " << name;
    thisTarget = SCNetworkReachabilityCreateWithName( 0, name );

    if ( thisTarget == NULL ) {
        err = SCError();
    }

    if ( err != noErr ) {
        qWarning() << "Could not create reachability reference!";
        fprintf( stderr, "Failed with error %s\n.", SCErrorString( err ) );
        return true;
    }

    err = noErr;
    SCNetworkConnectionFlags flags;
    ok = SCNetworkReachabilityGetFlags( thisTarget, &flags );

    if ( ! ok ) {
        err = SCError();
    }

    if ( err != noErr ) {
        qWarning() << "An error occoured!";
        return true;
    }

    bool isReachable = false;

    if ( flags & kSCNetworkReachabilityFlagsReachable ) {
        isReachable = true;
    }

    // This flag indicates that the specified nodename or address can
    // be reached using the current network configuration, but a
    // connection must first be established.
    //
    // As an example, this status would be returned for a dialup
    // connection that was not currently active, but could handle
    // network traffic for the target system.
    bool noConnectionRequired = !( flags & kSCNetworkReachabilityFlagsConnectionRequired );
    bool available = ( isReachable && noConnectionRequired ) ? true : false;
    qDebug() << "Internet available: " << available;
    CFRelease( thisTarget );
    return available;
}

