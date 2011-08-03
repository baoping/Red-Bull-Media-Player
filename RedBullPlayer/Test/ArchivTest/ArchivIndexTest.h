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
#ifndef ARCHIVINDEXTEST_H
#define ARCHIVINDEXTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "../AutoTest.h"

class ArchivIndexTest : public QObject {
        Q_OBJECT

    private slots:
        void should_save_to_file();
        void should_add_file_to_index();
        void should_remove_file_from_index();
        void should_get_file_from_index();
        void should_get_file_count();
        void should_calculate_extracedcontentsize();
};

DECLARE_TEST( ArchivIndexTest )

#endif // ARCHIVINDEXTEST_H
