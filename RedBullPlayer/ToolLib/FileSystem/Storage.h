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
#ifndef STORAGE_H
#define STORAGE_H

// Qt includes
#include <QMap>


// Project includes
#include "../../Interfaces/FileSystem/IStorage.h"
#include "../../Interfaces/FileSystem/IFileSystem.h"
#include "../../Interfaces/FileSystem/IStorageReservation.h"
#include "../UuidCreator.h"
namespace RedBullPlayer {
    namespace Tools {
        class Storage : public IStorage {
            public:
                Storage();
                Storage( IFileSystem* fileSystem );

                /**
                 * \returns the IFileSystem object reference as internal used for free/used bytes calculations
                 */
                virtual const IFileSystem& fileSystem() const {
                    return *_fileSystem;
                }

                /**
                  * Reserve n bytes of stroage
                  *
                  * \param bytes number of bytes to reserve
                  * \param error Error object to reciever error message if an error occoured
                  * \returns A new created instance if IStorageReservation
                  */
                virtual IStorageReservation* reserveStorage( quint64 bytes, RedBullPlayer::IError* error );

                /**
                  * Cancels the storage reservation e.g. internally freeing the reserved space
                  * and deleting the reservation object
                  *
                  * \param reservation the Reservation object to cancel
                  * \param error Error object to reciever error message if an error occoured
                  */
                virtual void cancelReservation( IStorageReservation* reservation, RedBullPlayer::IError* error );

                /**
                  * Commits the storage reservation and deletes the reservation object.
                  *
                  * \param reservation the Reservation object to commit
                  * \param error Error object to reciever error message if an error occoured
                  */
                virtual void commitReservation( IStorageReservation* reservation, RedBullPlayer::IError* error );

                /**
                  * \returns The capacity for the undelieing filesystem
                  */
                virtual quint64 capacity();

                /**
                  * \returns The number of available bytes for the current user
                  */
                virtual quint64 freeBytes();

                /**
                  * \returns The number of really used of the filesystem
                  */
                virtual quint64 usedBytes();

                /**
                  * \returns The sum of all reserved bytes
                  */
                virtual quint64 reservedBytes();

                /**
                  * Set the Application buffer size. e.g the amount of bytes that are allways reserverd
                  * for misc application writes
                  *
                  * \param applicationBufferSize the new size of the applicationBuffer
                  */
                virtual void setApplicationBufferSize( quint64 applicationBufferSize ) {
                    _applicationBufferSize = applicationBufferSize;
                }

                /**
                  * Get the cutrrent Application buffer size
                  *
                  * \returns the current application buffer size
                  */
                virtual quint64 applicationBufferSize() const {
                    return _applicationBufferSize;
                }

            protected:
                quint64 calculateAvailableBytes();

            private:
                quint64 _applicationBufferSize;
                QMap<QUuid, IStorageReservation*> _reservations;
                IFileSystem* _fileSystem;


                class StorageReservation : public RedBullPlayer::Tools::IStorageReservation {
                    public:
                        StorageReservation( quint64 reservedBytes ) {
                            _id = RedBullPlayer::Tools::UuidCreator::create();
                            _reservedBytes = reservedBytes;
                        }

                        /**
                         * \returns the id of the reservation
                         */
                        virtual QUuid id() const {
                            return _id;
                        }

                        /**
                         * \returns The number of reserved bytes of the reservation
                         */
                        virtual quint64 reservedBytes() const {
                            return _reservedBytes;
                        }

                    private:
                        QUuid _id;
                        quint64 _reservedBytes;
                };
        };

    } // namespace Tools
} // namespace RedBullPlayer

#endif // STORAGE_H
