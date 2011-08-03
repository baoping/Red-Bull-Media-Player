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
#include "StorageTest.h"


#include "Error.h"
#include "IError.h"
#include "FileSystem/Storage.h"
#include "FileSystem/IFileSystem.h"
#include "FileSystem/IStorageReservation.h"
#include "FileSystemFake.h"


using namespace RedBullPlayer;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

void StorageTest::should_initialise_filesystem() {
    Storage* s = new Storage();
    QVERIFY2( &s->fileSystem() != 0, "File System not initialzed" );
}

void StorageTest::should_use_filesystem_from_ctor() {
    FileSystemFake fs( "./" );
    Storage* s = new Storage( &fs );
    QVERIFY2( &s->fileSystem() == &fs, "File System not correctly initialzed" );
}

void StorageTest::should_return_complete_freespace_wihtout_reservations() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1000 );
    Storage* s = new Storage(  &fs );
    qDebug() << "s->freeBytes()" << s->freeBytes();
    QVERIFY2( s->freeBytes() == 1000 , "Free Bytes not correctly returned" );
}

void StorageTest::should_accept_reservation_when_enough_freespace_available() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1000 );
    Storage* s = new Storage(  &fs );
    Error err;
    err.setErrorMessage( "bla" );
    IStorageReservation* reservation = s->reserveStorage( 500, &err );
    QVERIFY2( err.errorType() == IError::NoError, "Error object must not hold an error" );
    QVERIFY2( reservation != 0, "Reservation mut be a vailld alocated object" );
    QVERIFY2( reservation->reservedBytes() == 500, "Reservation must contain the reserved bytes count" );
}

void StorageTest::should_reject_reservation_if_not_enough_space_available() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 100 );
    Storage* s = new Storage(  &fs );
    Error err;
    IStorageReservation* reservation = s->reserveStorage( 500, &err );
    QVERIFY2( err.errorType() == IError::Normal, "Error object must not hold an error" );
    QVERIFY2( err.errorMessage() == tr( "Not enougth space left on device" ), "Error message not correct" );
    QVERIFY2( reservation == 0, "Reservation mut not be a vaild allocated object" );
}

void StorageTest::should_reject_reservation_if_not_enough_space_available_because_of_application_buffer() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 500 );
    Storage* s = new Storage(  &fs );
    s->setApplicationBufferSize( 1 );
    Error err;
    IStorageReservation* reservation = s->reserveStorage( 500, &err );
    QVERIFY2( err.errorType() == IError::Normal, "Error object must not hold an error" );
    QVERIFY2( err.errorMessage() == tr( "Not enougth space left on device" ), "Error message not correct" );
    QVERIFY2( reservation == 0, "Reservation mut not be a vaild allocated object" );
}

void StorageTest::should_decrease_freebytes_and_increse_reservedbytes_afert_succesful_reservation() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1200 );
    Storage* s = new Storage(  &fs );
    Error err;
    err.setErrorMessage( "bla" );
    s->reserveStorage( 500, &err );
    QVERIFY2( s->freeBytes() == 700, "Freebytes must be decreased by the reserved bytes count" );
    QVERIFY2( s->reservedBytes() == 500, "Reserved must be increased by the reserved bytes count" );
    s->reserveStorage( 300, &err );
    QVERIFY2( s->freeBytes() == 400, "Freebytes must be decreased by the reserved bytes count" );
    QVERIFY2( s->reservedBytes() == 800, "Reserved must be increased by the reserved bytes count" );
    IStorageReservation* reservation3 = s->reserveStorage( 500, &err );
    QVERIFY2( reservation3 == 0, "Reservation must fail if not enogh space left" );
    QVERIFY2( s->freeBytes() == 400, "Freebytes must not be decreased when reservation fails" );
    QVERIFY2( s->reservedBytes() == 800, "Reserved must not be increased  when reservation fails" );
}

void StorageTest::should_decrease_reservedbytes_when_commiting_reservation() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1200 );
    Storage* s = new Storage(  &fs );
    Error err;
    err.setErrorMessage( "bla" );
    IStorageReservation* reservation = s->reserveStorage( 500, &err );
    QVERIFY2( s->freeBytes() == 700, "Freebytes must be decreased by the reserved bytes count" );
    QVERIFY2( s->reservedBytes() == 500, "Reserved must be increased by the reserved bytes count" );
    s->commitReservation( reservation, &err );
    QVERIFY2( err.errorType() == IError::NoError, "Error must not be set" );
    QVERIFY2( s->reservedBytes() == 0, "Reserved must be decreased by commiting the reservation" );
}

void StorageTest::should_decrease_reservedbytes_when_canceling_reservation() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1200 );
    Storage* s = new Storage(  &fs );
    Error err;
    err.setErrorMessage( "bla" );
    IStorageReservation* reservation = s->reserveStorage( 500, &err );
    QVERIFY2( s->freeBytes() == 700, "Freebytes must be decreased by the reserved bytes count" );
    QVERIFY2( s->reservedBytes() == 500, "Reserved must be increased by the reserved bytes count" );
    s->cancelReservation( reservation, &err );
    QVERIFY2( err.errorType() == IError::NoError, "Error must not be set" );
    QVERIFY2( s->reservedBytes() == 0, "Reserved must be decreased by commiting the reservation" );
}

void StorageTest::should_return_filesystem_capacity() {
    FileSystemFake fs( "./" );
    fs.setCapacity( 1200 );
    Storage* s = new Storage(  &fs );
    QVERIFY2( s->capacity() == 1200, "Must return filesystem capacity" );
}

void StorageTest::should_return_filesystem_used_bytes() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 1200 );
    fs.setUsedBytes( 1000 );
    Storage* s = new Storage(  &fs );
    QVERIFY2( s->usedBytes() == 1000, "Must return filesystem used bytes" );
}

void StorageTest::should_return_complete_freespace_wihtout_reservations_and_application_buffer() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 50000 );
    Storage* s = new Storage(  &fs );
    s->setApplicationBufferSize( 10 * 1024 );
    QVERIFY2( s->freeBytes() == ( 50000 - ( 10 * 1024 ) ), "application buffer size must be substracted from the free size" );
}


void StorageTest::should_initialize_get_and_set_application_buffer_size() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 50000 );
    Storage* s = new Storage(  &fs );
    QVERIFY2( s->applicationBufferSize() == 0, "Application buffer must be initilaized with 0" );
    s->setApplicationBufferSize( 10 );
    QVERIFY2( s->applicationBufferSize() == 10, "Application buffer size must be set correctly" );
}


void StorageTest::should_be_able_to_handle_uint64_underun_situation() {
    FileSystemFake fs( "./" );
    fs.setFreeBytes( 100 );
    Storage* s = new Storage(  &fs );
    s->setApplicationBufferSize( 101 );
    QVERIFY2( s->freeBytes() == 0, "Must be abkle to handle unit underun situation" );
    s->setApplicationBufferSize( 201 );
    QVERIFY2( s->freeBytes() == 0, "Must be abkle to handle unit underun situation" );
    s->setApplicationBufferSize( 100 );
    QVERIFY2( s->freeBytes() == 0, "Must be abkle to handle unit underun situation" );
}

