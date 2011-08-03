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

// QT includes
#include <QCoreApplication>
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QLocale>

// Project Includes
#include "UpdateEngine.h"
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"
#include "../Widgets/UpdateProgressWidget.h"

using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Widgets;
#ifdef Q_WS_MAC
#include <mach-o/dyld.h>
#endif

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
    a.setApplicationName( "RedBullUpdater" );
    a.setApplicationVersion( "1.0.0.0" );
    a.setOrganizationName( "mii GmbH" );
    a.setOrganizationDomain( "www.mii.at" );
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATER, QString( "Update Application started" ) );
    QStringList cmdline_args = QCoreApplication::arguments();

    if ( cmdline_args.count() < 5 ) {
        qWarning() << "Invalid number of Arguments";
        qWarning() << "Usage: updater.exe fromVersion toVersion updateFilePath originalApplicationPath" << endl;
        SimpleLogger::instance()->error( Constants::LOGGER_UPDATER, QString( "Invalid number of Arguments" ) );
        return 1;
    }

    UpdateProgressWidget *w = new UpdateProgressWidget( QApplication::activeWindow() );
    w->setHeader( "Initialising update..." );
    w->show();
    w->raise();
    UpdateEngine engine( &a );
    engine.prepareExecute( w, cmdline_args.at( 1 ), cmdline_args.at( 2 ), cmdline_args.at( 3 ), cmdline_args.at( 4 ) );
    engine.start( QThread::NormalPriority );
    return a.exec();
}


