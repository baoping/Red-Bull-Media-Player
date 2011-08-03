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
#include "WorkerThread.h"

// #include "../QCompress/Archiv.h"
#include <QFile>
#include <QWidget>
#include <QWidget>
#include <QtGui/QApplication>
#include <QRect>
#include <QPoint>
#include <QFrame>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QApplication>

#include "Package.h"
#include "FileSystem.h"
#include "RedBullDialog.h"

using namespace std;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Widgets;

WorkerThread::WorkerThread( QFileInfo archFile, QString hiddenDir, SplashScreen* splash ) {
    _archFile = archFile;
    _hiddenDir = hiddenDir;
    _splash = splash;
    _progress = 0;
}


void WorkerThread::startExtracting( int fileCount ) {
    _splash->initializeProgress( 0, fileCount, 1 );
    QApplication::processEvents();
}
void WorkerThread::extractingFile( QString fileName ) {
    _splash->progress( ++_progress, QString( "Extracting file: %1 ..." ).arg( QFileInfo( fileName ).baseName() ) );
    QApplication::processEvents();
}
void WorkerThread::checkingFile( QString fileName ) {
    _splash->setProgressText( QString( "Checking file: %1 ..." ).arg( QFileInfo( fileName ).baseName() ) );
    QApplication::processEvents();
}
void WorkerThread::skippingFile( QString fileName ) {
    _splash->progress( ++_progress, QString( "Checked file: %1" ).arg( QFileInfo( fileName ).baseName() ) );
    QApplication::processEvents();
}
void WorkerThread::finishExtracting() {
}

void WorkerThread::packageDeleted() {
}

void WorkerThread::showDiskspaceError() {
    RedBullDialog* diag = new RedBullDialog(
        tr( "<qt><p><b>Please free up some diskspace to start the Red Bull Media Player!</b></p></qt>" ), "", RedBullDialog::Ok, _splash );
    diag->show();
    diag->move( ( int )(  QApplication::desktop()->geometry().width() / 2 ) - ( diag->width() / 2 ),
                ( int )(  QApplication::desktop()->geometry().height() / 2 ) - ( diag->height() / 2 ) );
    QApplication::processEvents();
    diag->exec();
    QApplication::exit();
}

void WorkerThread::run() {
    // If you wish to add post-extraction code, do it here.
    connect( this, SIGNAL( requestDiskspaceError() ),
             this, SLOT( showDiskspaceError() ) );
    // FIXME: Remove Test code for dialog over splash screen.
    //QThread::currentThread()->sleep( 1 );
    //emit requestDiskspaceError();
    //return;
    QDir d( _hiddenDir );

    if ( ! QFile::exists( QDir::cleanPath( _hiddenDir + "/version.nfo" ) ) ) {
        Package* pkg = Package::openEdit( _archFile );

        if ( ! checkAvailoableDiskSpace( pkg->requiredDiskSpace() ) ) {
            delete pkg;
            return;
        }

        connect( pkg, SIGNAL( checkingFile( QString ) ),
                 this, SLOT( checkingFile( QString ) ) );
        connect( pkg, SIGNAL( extractingFile( QString ) ),
                 this, SLOT( extractingFile( QString ) ) );
        connect( pkg, SIGNAL( skippingFile( QString ) ),
                 this, SLOT( skippingFile( QString ) ) );
        connect( pkg, SIGNAL( finishExtracting() ),
                 this, SLOT( finishExtracting() ) );
        connect( pkg, SIGNAL( startExtracting( int ) ),
                 this, SLOT( startExtracting( int ) ) );
        connect( pkg, SIGNAL( destroyed( QObject* ) ),
                 this, SLOT( packageDeleted() ) );
        pkg->extractAll( _hiddenDir );
        delete pkg;
    } else {
        // _splash->initializeProgress( 0, 1, 1 );
        // QApplication::processEvents();
        // _splash->progress( 1, QString("") );
        QApplication::processEvents();
    }

    QProcess p;
    // p.setWorkingDirectory( d.absolutePath() );
#if defined(Q_OS_WIN)
    cout << "Starting app " << _hiddenDir.toUtf8().constData() << "Player.exe" << endl;
    p.startDetached( _hiddenDir + "Player.exe", QStringList() << _archFile.absoluteFilePath(), d.absolutePath() );
#endif
#if defined(Q_OS_LINUX)
    cout << "Starting app " << _hiddenDir.toUtf8().constData() << "mediaplayer.com" << endl;
    setAllPermissions( _hiddenDir + "Player.com" );
    setAllPermissions( _hiddenDir + "mediaplayer.com" );
    QProcess::startDetached( d.absolutePath() + "/mediaplayer.com", QStringList() << _archFile.absoluteFilePath(), d.absolutePath() );
#endif
    QThread::currentThread()->sleep( 1 );
    QApplication::exit();
}

bool WorkerThread::checkAvailoableDiskSpace( quint64 requiredSize ) {
    FileSystem fs( QFileInfo( "./" ).absolutePath() );
    cout << "Required size: " << requiredSize << endl;
    cout << "Capacity size: " << fs.capacity() << endl;
    cout << "Free bytes: " << fs.freeBytes() << endl;
    cout << "Used bytes: " << fs.usedBytes() << endl;

    if ( requiredSize >= fs.freeBytes() ) {
        emit requestDiskspaceError();
        return false;
    }

    return true;
}

#if defined(Q_OS_WIN)
WCHAR *WorkerThread::qStringToWideChar( const QString &str ) {
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
void WorkerThread::setPathEnvironmentVariable( QProcess &p, QStringList dirs, QString pwd ) {
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

void WorkerThread::setAllPermissions( QString fileName ) {
    QFile f( fileName );
    QFlags<QFile::Permission> newPerms;
    newPerms = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
               QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
               QFile::ReadUser | QFile::WriteUser | QFile::ExeUser |
               QFile::ReadOther | QFile::WriteOther | QFile::ExeOther;
    bool retVal = f.setPermissions( newPerms );
    qDebug() << "setting permissions for" << fileName << "retVal = " << retVal;
    f.close();
}

