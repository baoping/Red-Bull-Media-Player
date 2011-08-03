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
#ifndef STYLEPARSERTEST_H
#define STYLEPARSERTEST_H

/**
 * StyleParser UnitTests
 * WippelDa 01.04.2010
 */

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class StyleParserTest : public QObject {
        Q_OBJECT
    public:
        StyleParserTest();
    private slots:
        void test_file_should_exists();
        void should_have_any_textStyles();
        void should_have_3_textStyles();
        void should_have_SearchUI_HeaderText_style();
        void should_have_SearchUI_HeaderText_and_all_values();
        void should_have_CoverView_HeaderText_and_Tahoma_as_font();
};

DECLARE_TEST( StyleParserTest )
#endif // STYLEPARSERTEST_H
