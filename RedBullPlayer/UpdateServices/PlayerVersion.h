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
#ifndef PLAYERVERSION_H
#define PLAYERVERSION_H

#include <QObject>

#include "Version.h"

namespace RedBullPlayer {
    namespace Updater {
        class PlayerVersion : public QObject {
                Q_OBJECT
            public:
                static PlayerVersion* instance();

                PlayerVersion( QObject* parent );
                PlayerVersion( QObject* parent, QString fileName );
                PlayerVersion( QString versionString, QObject* parent );
                ~PlayerVersion();

                QString getMajorVersion() const {
                    return  _version-> majorVersion() == -1 ? "0" :  QString().setNum( _version-> majorVersion() );
                }

                QString getMinorVersion() const {
                    return   _version-> minorVersion() == -1 ? "0" : QString().setNum( _version-> minorVersion() );
                }

                QString getBuild() const {
                    return   _version-> build() == -1 ? "0" : QString().setNum( _version-> build() );
                }

                QString getTeamCityBuildNumber() const {
                    return _version-> teamCityBuildNumber() == -1 ? "0" :  QString().setNum( _version-> teamCityBuildNumber() );
                }

                QString getVersionString();
                QString internalVersionString();


                void parseString( QString* string );


                bool operator<( PlayerVersion &other );
                bool operator<=( PlayerVersion &other );
                bool operator==( PlayerVersion &other );
                bool operator>( PlayerVersion &other );
                bool operator>=( PlayerVersion &other );

                const Version& version() const {
                    return *_version;
                }

            private:
                static PlayerVersion* _instance;

                Version* _version;
                void initialize( QString fileName );
                void parseVersionString( QString versionString );
        };
    }
}

#endif // PLAYERVERSION_H
