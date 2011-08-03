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
#ifndef UPDATEMANAGERTEST_H
#define UPDATEMANAGERTEST_H

#include <QObject>
#include <QString>
#include <QtTest/QtTest>

#include "../AutoTest.h"

#include "Version.h"
#include "PlayerVersion.h"
#include "UpdateManager.h"


class UpdateManagerTest : public QObject {
        Q_OBJECT
    private slots:
        void should_initialize_with_empty_process_list();
        void should_add_process_and_sort_correctly();
        void should_resolve_required_processes();
};
DECLARE_TEST( UpdateManagerTest )
#endif // UPDATEMANAGERTEST_H
