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
#include "SelfExtract.h"

#include <QFile>
#include <QWidget>
#include <QtGui/QApplication>
#include <QRect>
#include <QPoint>
#include <QFrame>
#include <QDesktopWidget>
#include <QPixmap>
#include <QSplashScreen>

#include "../../RedBullPlayer/Player/splashscreen.h"
#include "WorkerThread.h"

#if defined(Q_OS_LINUX)
extern "C" {
    /* ----------------------------------------------------------------------- *
    *
    *   Copyright 2005 H. Peter Anvin - All Rights Reserved
    *
    *   This program is free software; you can redistribute it and/or modify
    *   it under the terms of the GNU General Public License as published by
    *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
    *   Boston MA 02111-1307, USA; either version 2 of the License, or
    *   (at your option) any later version; incorporated herein by reference.
    *
    * ----------------------------------------------------------------------- */

    /*
    * fatattr.c
    *
    * Display or change attributes on a FAT filesystem, similar to the
    * DOS attrib command.
    */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/msdos_fs.h>

#ifndef FAT_IOCTL_GET_ATTRIBUTES
# define FAT_IOCTL_GET_ATTRIBUTES        _IOR('r', 0x10, __u32)
#endif
#ifndef FAT_IOCTL_SET_ATTRIBUTES
# define FAT_IOCTL_SET_ATTRIBUTES        _IOW('r', 0x11, __u32)
#endif

    /* Currently supports only the FAT flags, not the NTFS ones */
    const char bit_to_char[] = "rhsvda67";

    static int chattr( const char *file, uint32_t s_attr, uint32_t c_attr ) {
        int fd = -1;
        uint32_t attr, nattr;
        int e;
        fd = open( file, O_RDONLY );

        if ( fd < 0 )
            goto err;

        if ( ioctl( fd, FAT_IOCTL_GET_ATTRIBUTES, &attr ) )
            goto err;

        nattr = ( attr & ~c_attr ) | s_attr;

        if ( nattr != attr ) {
            if ( ioctl( fd, FAT_IOCTL_SET_ATTRIBUTES, &nattr ) )
                goto err;
        }

        close( fd );
        return 0;
err:
        e = errno;

        if ( fd >= 0 )
            close( fd );

        errno = e;
        return -1;
    }

    static uint32_t parse_attr( const char *attrs ) {
        uint32_t attr = 0;
        const char *p;

        while ( *attrs ) {
            p = strchr( bit_to_char, *attrs );

            if ( !p ) {
                // We simply ignore this error - only result of it would be that the folder stays visible on win32
                return 0;
            }

            attr |= ( uint32_t )1 << ( p - bit_to_char );
            attrs++;
        }

        return attr;
    }
}
#endif // defined(Q_OS_LINUX)

using namespace std;

SplashScreen* splash = 0;
int main( int argc, char** argv ) {
    QApplication a( argc, argv );
    openSplashScreen();
    cout << "Self Extract Archive : " << argv[0] << endl;
    QString hiddenDir = "";
#if defined(Q_OS_WIN)
    hiddenDir = "./.bin_win/";
#endif
#if defined(Q_OS_LINUX)
    hiddenDir = "./.bin_linux/";
#endif
    CreateHiddenDirectory( hiddenDir.toUtf8().constData() );
    cout << "Start Worker Thread";
    QFileInfo inf( argv[0] );
    WorkerThread wt( inf, hiddenDir, splash );
    wt.start( QThread::NormalPriority );
    // Return success status
    return a.exec();
    // return 0;
}

void openSplashScreen() {
    splash = new SplashScreen();
    splash->setWindowState( Qt::WindowFullScreen );
    splash->show();
    QApplication::processEvents();
    splash->setWindowSize();
    QApplication::processEvents();
    splash->raise();
    QApplication::processEvents();
}

void CreateHiddenDirectory( const char* name ) {
    qDebug() << "Setup Directory: " <<  name ;
    QDir d( name );

    if ( QFile::exists( QDir::cleanPath( QString( name ) + "/force_reinstall" ) )
            && d.exists() ) {
        ReursiveDeleteDirectory( name );
    }

    QString path = d.absolutePath();

    if ( !d.exists() ) {
        QDir().mkpath( path );
    }

#if defined(Q_OS_LINUX)
    const char* fileName = path.toLatin1();
    chattr( fileName, parse_attr( "h" ), 0 );
#endif
#if defined(Q_OS_WIN)
    qDebug() << "Hide Directory: " <<  path ;
    qDebug() << "Hide Directory: " << QString::fromStdWString( qStringToWideChar( path ) );
    bool worked = SetFileAttributes( qStringToWideChar( path ), 0x2 );
    qDebug() << worked;
#endif
}

#if defined(Q_OS_WIN)
WCHAR *qStringToWideChar( const QString &str ) {
    if ( str.isNull() )
        return 0;

    WCHAR *result = new WCHAR[str.length() + 1];

    for ( int i = 0; i < str.length(); ++i ) {
        result[i] = str[i].unicode();
    }

    result[str.length()] = 0;
    return result;
}
#endif

#if defined(Q_OS_LINUX)
void setPathEnvironmentVariable( QProcess &p, QStringList dirs, QString pwd ) {
    QStringList env = QProcess::systemEnvironment();
    QString value = "";

    for( int i = 0; i < dirs.size(); i++ ) {
        value += dirs.at( i );

        if( i < dirs.size() - 1 ) {
            value += ":";
        }
    }

    env.append( "LD_LIBRARY_PATH=" + value );
    env.replaceInStrings( QRegExp( "^PWD=(.*)", Qt::CaseInsensitive ), "PWD=" + pwd );
    qDebug() << "setPathEnvironmentVariable: " << env;
    p.setEnvironment( env );
}
#endif

void ReursiveDeleteDirectory( const QString d ) {
    QDir dir( d );

    if ( dir.exists() ) {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;

        for ( int l = 0; l < list.size(); l++ ) {
            fi = list.at( l );

            if ( fi.isDir() && fi.fileName() != "." && fi.fileName() != ".." )
                ReursiveDeleteDirectory( fi.absoluteFilePath() );
            else if ( fi.isFile() ) {
                QFile f( fi.absoluteFilePath() );
                bool ret = f.remove();

                if ( !ret )
                    qDebug() << "Deleted file: " << fi.absoluteFilePath();
            }
        }

        qDebug() << "Deleted directory: " << dir.absolutePath();
        dir.rmdir( d );
    }
}

