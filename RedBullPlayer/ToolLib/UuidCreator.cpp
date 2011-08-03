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
#include "UuidCreator.h"

#include <QDebug>
#include <QFile>

using namespace RedBullPlayer::Tools;

/* Code taken from merge request: http://qt.gitorious.org/qt/qt/merge_requests/629 */
QUuid UuidCreator::create() {
#ifdef Q_OS_WIN
    return QUuid::createUuid();
#endif
    QUuid result;
    uint *data = &( result.data1 );
#ifdef Q_OS_UNIX
    QFile devUrandom;
    devUrandom.setFileName( QLatin1String( "/dev/urandom" ) );

    if ( devUrandom.open( QIODevice::ReadOnly ) ) {
        qint64 numToRead = 4 * sizeof( uint );
        devUrandom.read( ( char * ) data, numToRead ); // should read 128-bits of data
    } else
#endif
    {
        static const int intbits = sizeof( int ) * 8;
        static int randbits = 0;

        if ( !randbits ) {
            int max = RAND_MAX;

            do {
                ++randbits;
            } while ( ( max = max >> 1 ) );
        }

        // reseed, but only if not already seeded
        qsrand( rand() );
        int chunks = 16 / sizeof( uint );

        while ( chunks-- ) {
            uint randNumber = 0;

            for ( int filled = 0; filled < intbits; filled += randbits )
                randNumber |= qrand() << filled;

            *( data + chunks ) = randNumber;
        }
    }

    result.data4[0] = ( result.data4[0] & 0x3F ) | 0x80;      // UV_DCE
    result.data3 = ( result.data3 & 0x0FFF ) | 0x4000;      // UV_Random
    return result;
}
