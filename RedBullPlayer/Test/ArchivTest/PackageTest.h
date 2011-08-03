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
#ifndef PACKAGETEST_H
#define PACKAGETEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "../AutoTest.h"

class PackageTest : public QObject {
        Q_OBJECT

    private slots:
        void should_setup_path();
        void should_write_file_to();
        void should_build_package();
        void should_extract_package();
        void should_extract_file();
        void should_return_extracted_size();
};

DECLARE_TEST( PackageTest )


#endif // PACKAGETEST_H
