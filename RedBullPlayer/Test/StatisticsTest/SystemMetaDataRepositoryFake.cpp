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
#include "SystemMetaDataRepositoryFake.h"

using namespace RedBullPlayer::Modules::MediaLibrary;

SystemMetaDataRepositoryFake::SystemMetaDataRepositoryFake( QObject *parent ) : QObject( parent ) {
    _metaData.insert( "DBVersion", "1.0" );
    _metaData.insert( "PlayerUUID", "" );
    _metaData.insert( "CollectStatistic", "true" );
    _metaData.insert( "DownloadContent", "true" );
    _metaData.insert( "ShowUserSettingsDialog", "true" );
}

SystemMetaDataRepositoryFake::~SystemMetaDataRepositoryFake() {
    _metaData.insert( "DBVersion", "1.0" );
    _metaData.insert( "PlayerUUID", "" );
    _metaData.insert( "CollectStatistic", "true" );
    _metaData.insert( "DownloadContent", "true" );
    _metaData.insert( "ShowUserSettingsDialog", "true" );
};

QString SystemMetaDataRepositoryFake::getMetaData( const QString& dataKey ) {
    return _metaData.contains( dataKey ) ? _metaData.value( dataKey ) : "";
}

QString SystemMetaDataRepositoryFake::getDBVersion() {
    return "1.0";
}

void SystemMetaDataRepositoryFake::setPlayerUUID( QUuid id ) {
    _metaData["PlayerUUID"] = id.toString();
}

QUuid SystemMetaDataRepositoryFake::getPlayerUUID() {
    return QUuid( getMetaData( "PlayerUUID" ) );
}

bool SystemMetaDataRepositoryFake::allowCollectStatistic() {
    return ( getMetaData( "CollectStatistic" ) == "true" );
}

void SystemMetaDataRepositoryFake::setAllowCollectStatistic( bool collectStatistic ) {
    _metaData["CollectStatistic"] = ( collectStatistic ) ? "true" : false;
}

bool SystemMetaDataRepositoryFake::allowNetworkConnection() {
    return ( getMetaData( "DownloadContent" ) == "true" );
}

void SystemMetaDataRepositoryFake::setAllowNetworkConnection( bool downloadContent ) {
    _metaData["DownloadContent"] = ( downloadContent ) ? "true" : false;
}

bool SystemMetaDataRepositoryFake::getShowUserSettingsDialog() {
    return ( getMetaData( "ShowUserSettingsDialog" ) == "true" );
}

void SystemMetaDataRepositoryFake::setShowUserSettingsDialog( bool showUserSettingsDialog ) {
    _metaData["ShowUserSettingsDialog"] = ( showUserSettingsDialog ) ? "true" : false;
}

