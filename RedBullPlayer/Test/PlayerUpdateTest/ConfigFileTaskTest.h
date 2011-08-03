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
#ifndef CONFIGFILTASKTEST_H
#define CONFIGFILTASKTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"

class ConfigFileTaskTest : public QObject {
        Q_OBJECT

    private slots:
        //   void should_extract_config_ini_to_temp_folder_as_config_ini_new();
};

DECLARE_TEST( ConfigFileTaskTest )

#endif // CONFIGFILTASKTEST_H
