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
#ifndef SENDERQUEUETEST_H
#define SENDERQUEUETEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class SenderQueueTest : public QObject {
        Q_OBJECT

    private slots:
        void should_queue_new_request_when_internet_available();
        void should_not_queue_new_request_when_internet_available();

        void should_start_timer_in_ctor();
        void should_initialize_netwrok_access_manager_in_ctor();
        void when_the_timer_shots_should_unqueue_and_set_as_current_entry();
        void when_the_timer_shots_should_not_unqueue_and_set_as_current_entry_if_there_allready_is_one_set();
        void should_initialize_currentEntry_with_0() ;

        void should_process_all_when_no_request_fails();
        void should_process_all_when_some_request_fails();

        void should_generate_correct_url_using_factory();

        // void should_queue_cached_request_when_internet_becomes_available();
};

DECLARE_TEST( SenderQueueTest )

#endif // SENDERQUEUETEST_H
