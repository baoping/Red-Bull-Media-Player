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
#ifndef NEWSTEST_H
#define NEWSTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"


class NewsTest : public QObject {
        Q_OBJECT
    private slots:
        void should_validate_news();
        void should_invalidate_news_string_empty();
        void should_invalidate_news_string_null();

};
DECLARE_TEST( NewsTest )

#endif // NEWSTEST_H
