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
#include "Storage.h"

// QT includes
#include <QtGlobal>
#include <QObject>
#include <QLocale>
#include <QString>
#include <QDebug>

// Project includes
#include "FileSystem.h"
#include "../../Container/Constants.h"
#include "../SimpleLogger.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

Storage::Storage() : IStorage() {
    _fileSystem = new FileSystem( "./" );
    _applicationBufferSize = 0;
}

Storage::Storage( IFileSystem* fileSystem ) :  IStorage() {
    _fileSystem = fileSystem;
    _applicationBufferSize = 0;
}

IStorageReservation* Storage::reserveStorage( quint64 bytes, RedBullPlayer::IError* error ) {
    Q_ASSERT( error );
    qDebug() << Q_FUNC_INFO << ( int )bytes / 1024 << "kb";
    error->reset();

    if ( freeBytes() < bytes ) {
        error->setErrorMessage( QObject::tr( "Not enougth space left on device" ) );
        return 0;
    }

    IStorageReservation* reservation = new StorageReservation( bytes );
    _reservations[ reservation->id() ] = reservation;
    return reservation;
}

void Storage::cancelReservation( IStorageReservation* reservation, RedBullPlayer::IError* error ) {
    Q_ASSERT( reservation );
    Q_ASSERT( error );
    qDebug() << Q_FUNC_INFO << ( int )reservation->reservedBytes() / 1024 << "kb";
    error->reset();
    _reservations.remove( reservation->id() );
    delete reservation;
    reservation = 0;
}

void Storage::commitReservation( IStorageReservation* reservation, RedBullPlayer::IError* error )  {
    Q_ASSERT( reservation );
    Q_ASSERT( error );
    qDebug() << Q_FUNC_INFO << ( int )reservation->reservedBytes() / 1024 << "kb";
    error->reset();
    _reservations.remove( reservation->id() );
    delete reservation;
    reservation = 0;
}

quint64 Storage::capacity() {
    return _fileSystem->capacity();
}

quint64 Storage::freeBytes() {
    return calculateAvailableBytes();
}

quint64 Storage::usedBytes() {
    return _fileSystem->usedBytes();
}

quint64 Storage::reservedBytes() {
    IStorageReservation* reservation;
    quint64 sum = 0;
    foreach( reservation, _reservations ) {
        sum += reservation->reservedBytes();
    }
    return sum;
}

quint64 Storage::calculateAvailableBytes() {
    qDebug() << Q_FUNC_INFO;
    qDebug() << "physical free: " << ( int )_fileSystem->freeBytes() / 1024 << "kb";
    qDebug() << "applicationBufferSize: " << ( int )applicationBufferSize() / 1024 << "kb";
    qDebug() << "reservedBytes: " << ( int )reservedBytes() / 1024 << "kb";
    quint64 toSubstract =  applicationBufferSize() + reservedBytes();
    quint64 fsFreeBytes = _fileSystem->freeBytes();

    if ( toSubstract > fsFreeBytes ) {
        QString msg( QString( "Not enough space left on device to commit all pending reservations! "
                              "Required: %1kb available: %2kb" )
                     .arg( ( float )toSubstract / 1024 )
                     .arg( ( float )fsFreeBytes / 1024 ) );
        ;
        SimpleLogger::instance()->debug( Constants::LOGGER_ROOT, msg );
        qWarning() << msg;
        return 0;
    }

    return fsFreeBytes - toSubstract;
}

