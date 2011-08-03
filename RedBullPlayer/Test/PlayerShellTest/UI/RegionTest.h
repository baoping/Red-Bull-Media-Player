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
#ifndef REGIONTEST_H
#define REGIONTEST_H


#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class RegionTest : public QObject {
        Q_OBJECT

    private slots:
        void ctor_ArgumentPassing();
        // void registerRegion();
        // void findRegion();
};

DECLARE_TEST( RegionTest )

#endif // REGIONTEST_H
