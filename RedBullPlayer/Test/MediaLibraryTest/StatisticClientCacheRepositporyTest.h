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
#ifndef STATISTICCLIENTCACHEREPOSITPORYTEST_H
#define STATISTICCLIENTCACHEREPOSITPORYTEST_H


#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"


class StatisticClientCacheRepositporyTest : public QObject {
        Q_OBJECT
    private slots:
        void should_save_new_entry();
        void should_delete_entry();
        void should_getall_entries();
        void should_create_but_not_save_entry();
};
DECLARE_TEST( StatisticClientCacheRepositporyTest )



#endif // STATISTICCLIENTCACHEREPOSITPORYTEST_H
