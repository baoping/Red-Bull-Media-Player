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
#ifndef VERSION_H
#define VERSION_H

#include <QObject>
#include <QString>

class Version {
    public:
        Version( QString versionString );

        int majorVersion() const {
            return _majorVersion;
        }

        int minorVersion() const {
            return _minorVersion;
        }

        int build() const {
            return _build;
        }

        int teamCityBuildNumber() const {
            return _teamCityBuildNumber;
        }

        QString versionString() const;

        int compare( Version version ) const;
        void parseString( QString versionString );

    private:
        int compareEx( int a, int b ) const;

    private:
        int _majorVersion;
        int _minorVersion;
        int _build;
        int _teamCityBuildNumber;

};

#endif // VERSION_H
