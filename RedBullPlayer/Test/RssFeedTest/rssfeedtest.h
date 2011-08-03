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
#ifndef RSSFEEDTEST_H
#define RSSFEEDTEST_H

/**
 * RSSFeed UnitTests
 * WippelDa 01.04.2010
 */

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class RssFeedTest : public QObject {
        Q_OBJECT
    public:
        RssFeedTest();
    private slots:
        void test_file_should_exists();
        void should_have_any_feedItems();
        void should_have_3_feedItems();
        void should_have_defined_values();
        void should_have_items_and_defined_values();
};

DECLARE_TEST( RssFeedTest )
#endif // RSSFEEDTEST_H
