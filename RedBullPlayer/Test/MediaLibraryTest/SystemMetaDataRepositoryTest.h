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
#ifndef SYSTEMMETADATAREPOSITORYTEST_H
#define SYSTEMMETADATAREPOSITORYTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"


class SystemMetaDataRepositoryTest : public QObject {
        Q_OBJECT
    public:
        SystemMetaDataRepositoryTest();
    private slots:
        void should_get_db_version();
        void should_get_player_uuid();
        void should_get_collect_statistic();
        void should_get_download_content();
        void should_set_collect_statistic();
        void should_set_download_content();
        void should_get_show_user_settings_dialog();
        void should_set_show_user_settings_dialog();
};
DECLARE_TEST( SystemMetaDataRepositoryTest )

#endif // SYSTEMMETADATAREPOSITORYTEST_H
