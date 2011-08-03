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
#ifndef STATISTICSMODULETEST_H
#define STATISTICSMODULETEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class StatisticsModuleTest : public QObject {
        Q_OBJECT

    private slots:
        void should_react_on_internetconnection_changed_event();
        void should_set_internet_availability_for_sender_queue();
        void shouild_initialize_with_0_repository();
        void shouild_set_repository();
        void should_cache_to_repository_when_no_internet_available();
        void should_cache_to_repository_when_internet_available();
        void should_initialise_the_sender_queue();
        void should_react_to_create_statistic_entry_events();
        void should_queue_cached_requests_when_internet_becomes_available();
        void should_remove_cached_requests_only_when_sent_successfully();

};

DECLARE_TEST( StatisticsModuleTest )



#endif // STATISTICSMODULETEST_H
