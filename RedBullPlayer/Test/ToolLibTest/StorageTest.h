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
#ifndef STORAGETEST_H
#define STORAGETEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "../AutoTest.h"

class StorageTest : public QObject {
        Q_OBJECT

    private slots:
        void should_initialise_filesystem();
        void should_use_filesystem_from_ctor();
        void should_return_complete_freespace_wihtout_reservations();
        void should_accept_reservation_when_enough_freespace_available();
        void should_reject_reservation_if_not_enough_space_available();
        void should_reject_reservation_if_not_enough_space_available_because_of_application_buffer();
        void should_decrease_freebytes_and_increse_reservedbytes_afert_succesful_reservation();
        void should_decrease_reservedbytes_when_commiting_reservation();
        void should_decrease_reservedbytes_when_canceling_reservation();
        void should_return_filesystem_capacity();
        void should_return_filesystem_used_bytes();
        void should_return_complete_freespace_wihtout_reservations_and_application_buffer();
        void should_initialize_get_and_set_application_buffer_size();
        void should_be_able_to_handle_uint64_underun_situation();
};


DECLARE_TEST( StorageTest )


#endif // STORAGETEST_H
