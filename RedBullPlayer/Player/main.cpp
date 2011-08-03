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
#include <QtGui/QApplication>

// Qt Includes
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSplashScreen>

// Project Includes
#include "../PlayerShell/Shell.h"
#include "splashscreen.h"

#ifdef Q_WS_MAC
#include <mach-o/dyld.h>
#endif


using namespace RedBullPlayer::PlayerShell;

int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );
#ifdef Q_WS_MAC
    char path[2048];
    uint32_t size = sizeof( path );

    if ( _NSGetExecutablePath( path, &size ) == 0 ) {
        printf( "executable path is %s\n", path );
    } else {
        printf( "buffer too small; need size %u\n", size );
    }

    QFileInfo inf( path );
    qDebug() << "Set workingdir: " << inf.absolutePath();
    QDir::setCurrent( inf.absolutePath() );
#endif
    SplashScreen* s = new SplashScreen( 0 );
    // s->setWindowState( Qt::WindowFullScreen );
    s->show();
    s->setWindowSize();
    s->raise();
    Shell* shell = new Shell( 0 );
    shell->setProgressWidget( s->progressWidget() );
    shell->Bootstrap();
    shell->show();
    shell->setWindowSize();
    a.processEvents();
    s->close();
    shell->raise();
    return a.exec();
}
