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
#ifndef VERSIONTEST_H
#define VERSIONTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class VersionTest : public QObject {
        Q_OBJECT

    private slots:
        void should_be_lesser_major();
        void should_be_lesser_minor();
        void should_be_lesser_build();
        void should_be_lesser_tc();
        void should_be_equal();
        void should_be_greater_major();
        void should_be_greater_minor();
        void should_be_greater_build();
        void should_be_greater_tc();

        void should_be_fuzzy_equal_minor();
        void should_be_fuzzy_equal_build();
        void should_be_fuzzy_equal_tc();
};

DECLARE_TEST( VersionTest )

#endif // VERSIONTEST_H
