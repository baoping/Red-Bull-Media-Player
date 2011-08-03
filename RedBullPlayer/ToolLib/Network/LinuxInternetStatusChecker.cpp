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

// QT includes
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusArgument>

using namespace RedBullPlayer::Tools::Network;

InternetStatusChecker::~InternetStatusChecker() {}

bool InternetStatusChecker::internetAvailable() {
    // qDebug() << Q_FUNC_INFO;
    QDBusConnection bus = QDBusConnection::systemBus();

    if ( ! bus.isConnected() ) {
        qWarning() << "Could not connect to system bus!";
        return true;
    }

    QDBusInterface dbus_interface( "org.freedesktop.NetworkManager",
                                   "/org/freedesktop/NetworkManager",
                                   "org.freedesktop.DBus.Properties", bus );
    QDBusMessage reply = dbus_interface.call( "Get", "org.freedesktop.NetworkManager", "ActiveConnections" );
    // Make sure that result contains a reply.
    qDebug() << reply.type() ;

    if ( reply.type() != QDBusMessage::ReplyMessage ) {
        qWarning() << "DBus query failed! reply.type() != QDBusMessage::ReplyMessage";
        return true;
    }

    QVariant dv = qdbus_cast<QVariant>( reply.arguments().at( 0 ) );

    // If it is the reply we expect, it should be a QDBusArgument
    if ( ! dv.canConvert<QDBusArgument>() ) {
        qWarning() << "DBus query failed! ! reply.arguments().at(0).canConvert<QDBusArgument>()";
        return true;
    }

    // That contains an Array of QStrings holding our Device list.
    QDBusArgument myArg = dv.value<QDBusArgument>();

    if ( myArg.currentType() != QDBusArgument::ArrayType ) {
        qWarning() << "DBus Argument has wroing type! myArg.currentType() != QDBusArgument::ArrayType";
        return true;
    }

    myArg.beginArray();
    // Get memory for our resulting string list.
    QStringList myList;
    myList.clear(); // It should already be clear, but just to be safe.

    while ( !myArg.atEnd() ) {
        QString myElement = qdbus_cast<QString>( myArg );
        myList.append( myElement );
    }

    myArg.endArray();
    // qDebug()  << myList.join(",");
    bool availability = myList.count() > 0;
    qDebug()  << "Internet status: " << availability;
    return availability;
}
