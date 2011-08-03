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
#include "internetconnectionstatusprovider.h"

#include <QDebug>

#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::InternetConnectionChecker;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

InternetConnectionStatusProvider::InternetConnectionStatusProvider( QString referenceHost, int checkPeriod, QObject *parent ) :
    QObject( parent ) {
    _referenceHost = referenceHost;
    _checkPeriod = checkPeriod;
    bootstrap();
}

InternetConnectionStatusProvider::~InternetConnectionStatusProvider() {
    _checkTimer->stop();

    if( _connectionChecker )
        delete _connectionChecker;
}

void InternetConnectionStatusProvider::bootstrap() {
    _lastStatus = false;
    _firstTimeCheck =  true;
    _connectionChecker = new RedBullPlayer::Tools::Network::InternetStatusChecker( _referenceHost );
    _checkTimer = new QTimer( this );
    _checkTimer->setInterval( _checkPeriod );
    connect( _checkTimer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );
}

bool InternetConnectionStatusProvider::isRunning() {
    return _checkTimer->isActive();
}

void InternetConnectionStatusProvider::start() {
    _checkTimer->start();
}

void InternetConnectionStatusProvider::stop() {
    _checkTimer->stop();
}

void InternetConnectionStatusProvider::doCheck() {
    bool newStatus = _connectionChecker->internetAvailable();

    if ( _firstTimeCheck ) {
        emit statusChanged( newStatus );
        _firstTimeCheck = false;
    } else if ( ( _lastStatus != newStatus ) ) {
        emit statusChanged( newStatus );
    }

    _lastStatus = newStatus;
}

void InternetConnectionStatusProvider::onTimer() {
    doCheck();
}
