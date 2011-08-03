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
#include "PlayerVersion.h"

#include <QStringList>
#include <QFile>
#include <QDebug>

// Project includes
#include "Version.h"

using namespace RedBullPlayer::Updater;

PlayerVersion* PlayerVersion::_instance = 0;

PlayerVersion::PlayerVersion( QObject* parent ) : QObject( parent ) {
    //  _version = new Version( "0.0.0.0" );
    _version = 0;
    initialize( "version.nfo" );
}

PlayerVersion::PlayerVersion( QObject* parent, QString fileName ) : QObject( parent ) {
    //   _version = new Version( "0.0.0.0" );
    _version = 0;
    initialize( fileName );
}

PlayerVersion::PlayerVersion( QString versionString, QObject* parent ): QObject( parent ) {
    // _version = new Version( "0.0.0.0" );
    _version = 0;
    parseVersionString( versionString );
}

PlayerVersion:: ~PlayerVersion() {
    delete _version;
}

PlayerVersion* PlayerVersion::instance() {
    if ( _instance == 0 ) {
        _instance = new PlayerVersion( 0 );
    }

    Q_ASSERT( _instance != 0 );
    return _instance;
}

void PlayerVersion::initialize( QString fileName ) {
    if ( QFile::exists( fileName ) ) {
        QFile file( fileName );

        if ( file.open( QIODevice::ReadOnly ) ) {
            QString versionInfo = file.readAll();
            file.close();
            versionInfo = versionInfo.simplified(); //remove whitespaces
            parseVersionString( versionInfo );
        }
    }
}

void PlayerVersion::parseVersionString( QString versionString ) {
    if ( _version ) {
        delete _version;
    }

    _version = new Version( versionString );
    // _version->parseString( versionString );
}

QString PlayerVersion::getVersionString() {
    return QString( "%1.%2.%3.%4" )
           .arg( getMajorVersion() )
           .arg( getMinorVersion() )
           .arg( getBuild() )
           .arg( getTeamCityBuildNumber() );
}

QString PlayerVersion::internalVersionString() {
    return _version->versionString();
}

void PlayerVersion::parseString( QString* string ) {
    string->replace( "{#MajorVersion}", getMajorVersion() );
    string->replace( "{#MinorVersion}", getMinorVersion() );
    string->replace( "{#Build}", getBuild() );
    string->replace( "{#Revision}", getTeamCityBuildNumber() );
    string->replace( "{#VersionString}", getVersionString() );
}


bool PlayerVersion::operator<( PlayerVersion &other ) {
    return _version->compare( other.version() ) < 0;
}

bool PlayerVersion::operator<=( PlayerVersion &other ) {
    return _version->compare(  other.version() ) <= 0;
}

bool PlayerVersion::operator==( PlayerVersion &other ) {
    return _version->compare(  other.version() ) == 0;
}

bool PlayerVersion::operator>( PlayerVersion &other ) {
    return _version->compare(  other.version() ) > 0;
}

bool PlayerVersion::operator>=( PlayerVersion &other ) {
    return _version->compare(  other.version() ) >= 0;
}

