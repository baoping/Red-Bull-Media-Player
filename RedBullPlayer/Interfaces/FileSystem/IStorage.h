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
#ifndef ISTORAGE_H
#define ISTORAGE_H

#include "qglobal.h"

// Project Includes
#include "IFileSystem.h"
#include "IStorageReservation.h"
#include "../../Container/IError.h"

namespace RedBullPlayer {
    namespace Tools {
        /**
         * Class to manage storage reservations
         */
        class IStorage {
            public:
                virtual ~IStorage() {}

                /**
                 * \returns the IFileSystem object reference as internal used for free/used bytes calculations
                 */
                virtual const IFileSystem& fileSystem() const = 0;

                /**
                  * Reserve n bytes of stroage
                  *
                  * \param bytes number of bytes to reserve
                  * \param error Error object to reciever error message if an error occoured
                  * \returns A new created instance if IStorageReservation
                  */
                virtual IStorageReservation* reserveStorage( quint64 bytes, RedBullPlayer::IError* error ) = 0;

                /**
                  * Cancels the storage reservation e.g. internally freeing the reserved space
                  * and deleting the reservation object
                  *
                  * \param reservation the Reservation object to cancel
                  * \param error Error object to reciever error message if an error occoured
                  */
                virtual void cancelReservation( IStorageReservation* reservation, RedBullPlayer::IError* error ) = 0;

                /**
                  * Commits the storage reservation and deletes the reservation object.
                  *
                  * \param reservation the Reservation object to commit
                  * \param error Error object to reciever error message if an error occoured
                  */
                virtual void commitReservation( IStorageReservation* reservation, RedBullPlayer::IError* error ) = 0;

                /**
                  * \returns The capacity for the undelieing filesystem
                  */
                virtual quint64 capacity() = 0;

                /**
                  * \returns The number of available bytes for the current user
                  */
                virtual quint64 freeBytes() = 0;

                /**
                  * \returns The number of really used of the filesystem
                  */
                virtual quint64 usedBytes() = 0;

                /**
                  * \returns The sum of all reserved bytes
                  */
                virtual quint64 reservedBytes() = 0;

                /**
                  * Set the Application buffer size. e.g the amount of bytes that are allways reserverd
                  * for misc application writes
                  *
                  * \param applicationBufferSize the new size of the applicationBuffer
                  */
                virtual void setApplicationBufferSize( quint64 applicationBufferSize ) = 0;

                /**
                  * Get the cutrrent Application buffer size
                  *
                  * \returns the current application buffer size
                  */
                virtual quint64 applicationBufferSize() const = 0;
        };
    }
}


#endif // ISTORAGE_H
