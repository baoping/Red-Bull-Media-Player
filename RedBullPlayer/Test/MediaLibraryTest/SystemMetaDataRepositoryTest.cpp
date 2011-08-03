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
#include "SystemMetaDataRepositoryTest.h"

#include <QString>
#include <QDebug>
#include <QUuid>
#include <QImage>

#include "MediaLibrary/IMediaDB.h"
#include "MediaLibrary/ISystemMetaDataRepository.h"
#include "Shell.h"
#include "Error.h"
#include "SystemMetaDataRepository.h"
#include "MediaDB.h"
#include "MediaDB_sql.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;

SystemMetaDataRepositoryTest::SystemMetaDataRepositoryTest() {
}

void SystemMetaDataRepositoryTest::should_get_db_version() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    QString dbVersion = rep->getDBVersion();
    QVERIFY ( !dbVersion.isEmpty() );
    QVERIFY( dbVersion == DB_VERSION_METADATA_DATAVALUE );
}

void SystemMetaDataRepositoryTest::should_get_player_uuid() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    QUuid id = rep->getPlayerUUID();
    QVERIFY2( ! id.isNull(), "Player UUid must  not be null" );
}

void SystemMetaDataRepositoryTest::should_get_collect_statistic() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    bool retVal = rep->allowCollectStatistic();
    QVERIFY2( !retVal, "CollectStatistic should be false!" );
}

void SystemMetaDataRepositoryTest::should_get_download_content() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    bool retVal = rep->allowNetworkConnection();
    QVERIFY2( !retVal, "DownloadContent should be false!" );
}

void SystemMetaDataRepositoryTest::should_set_collect_statistic() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    rep->setAllowCollectStatistic( true );
    bool retVal = rep->allowCollectStatistic();
    QVERIFY2( retVal, "CollectStatistic should be true!" );
    rep->setAllowCollectStatistic( false );
    retVal = rep->allowCollectStatistic();
    QVERIFY2( !retVal, "CollectStatistic should be false!" );
}

void SystemMetaDataRepositoryTest::should_set_download_content() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    rep->setAllowNetworkConnection( true );
    bool retVal = rep->allowNetworkConnection();
    QVERIFY2( retVal, "DownloadContent should be true!" );
    rep->setAllowNetworkConnection( false );
    retVal = rep->allowNetworkConnection();
    QVERIFY2( !retVal, "DownloadContent should be false!" );
}

void SystemMetaDataRepositoryTest::should_get_show_user_settings_dialog() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    bool retVal = rep->getShowUserSettingsDialog();
    QVERIFY2( retVal, "ShowUserSettingsDialog should be true!" );
}

void SystemMetaDataRepositoryTest::should_set_show_user_settings_dialog() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    ISystemMetaDataRepository* rep = db->systemMetaDataRepository();
    rep->setShowUserSettingsDialog( true );
    bool retVal = rep->getShowUserSettingsDialog();
    QVERIFY2( retVal, "ShowUserSettingsDialog should be true!" );
    rep->setShowUserSettingsDialog( false );
    retVal = rep->getShowUserSettingsDialog();
    QVERIFY2( !retVal, "ShowUserSettingsDialog should be false!" );
}
