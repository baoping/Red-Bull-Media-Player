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
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QMainWindow>
#include <QTextEdit>
#include <QString>
#include <QSettings>



#include <stdio.h>
#include <stdlib.h>

#include "../ToolLib/SimpleLogger.h"
#include "../Interfaces/IError.h"
#include "../Container/Error.h"

#include "InputParser.h"
#include "CommandProcessor.h"
#include "ArgumentParser.h"


#ifdef Q_WS_MAC
#include <ApplicationServices/ApplicationServices.h>
#include "/Developer/Headers/FlatCarbon/Processes.h"
#endif

using namespace RedBullPlayer::Tools;

//static QMainWindow* mainWindow;
//static QTextEdit* myTextEdit;

void myMessageOutput( QtMsgType type, const char *msg ) {
    switch ( type ) {
        case QtDebugMsg:
            //myTextEdit->append( QString("Debug: %1\n").arg(msg) );
            fprintf( stderr, "Debug: %s\n", msg );
            break;

        case QtWarningMsg:
            //myTextEdit->append( QString("Warning: %1\n").arg(msg) );
            fprintf( stderr, "Warning: %s\n", msg );
            break;

        case QtCriticalMsg:
            //myTextEdit->append( QString("Critical: %1\n").arg(msg) );
            fprintf( stderr, "Critical: %s\n", msg );
            break;

        case QtFatalMsg:
            fprintf( stderr, "Fatal: %s\n", msg );
            abort();
    }
}



int main( int argc, char *argv[] ) {
    QCoreApplication a( argc, argv );
    QSettings* s = new QSettings( "config.ini", QSettings::IniFormat, 0 );
    bool ok;
    int loggingEnabled = s->value( "Logging/Enabled", 0 ).toInt( &ok );

    if ( !( ok && ( loggingEnabled == 1 ) ) )
        SimpleLogger::instance()->disable();

    delete s;

    /*
    #ifdef Q_WS_MAC
        ProcessSerialNumber psn;
        if (GetCurrentProcess(&psn) == noErr) {
    	TransformProcessType(&psn, kProcessTransformToForegroundApplication);
        }
    #endif
    */
    //  qInstallMsgHandler(myMessageOutput);

    // RedBullPlayer::SnapshotCreator::InputParser* p = new RedBullPlayer::SnapshotCreator::InputParser(0);
    // p->run();
    //RedBullPlayer::SnapshotCreator::CommandProcessor cp(0);
    // qDebug() << cp-

    if ( argc < 2 ) {
        qFatal( "No input gibven!" );
    }

    RedBullPlayer::SnapshotCreator::ArgumentParser ap( 0 );
    RedBullPlayer::Container::Error error;
    RedBullPlayer::SnapshotCreator::SnapshotCreationData* scd = ap.parseCommand( argv[1], &error );

    if ( error.errorType() == RedBullPlayer::IError::NoError ) {
        RedBullPlayer::SnapshotCreator::CommandProcessor* cp = new  RedBullPlayer::SnapshotCreator::CommandProcessor( 0 );
        cp->requestSnapshot( scd );
    } else {
        qFatal( error.errorMessage().toLatin1() );
    }

    return a.exec();
}
