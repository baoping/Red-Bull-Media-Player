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
#ifndef MEDIAWATCHERTEST_H
#define MEDIAWATCHERTEST_H

/**
 * MediaWatcher UnitTests
 * WippelDa 01.04.2010
 */

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class MediaWatcherTest : public QObject {
        Q_OBJECT
    public:
        MediaWatcherTest();
    private slots:
        void initTestCase();
        void cleanupTestCase();

        void should_watch_path_and_emit_file_added();
        void should_watch_path_and_emit_file_removed();

};

DECLARE_TEST( MediaWatcherTest )

#endif // MEDIAWATCHERTEST_H
