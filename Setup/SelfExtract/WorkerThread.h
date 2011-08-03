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
#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QDebug>
#include <iostream>
#include <QProcess>
#include <QFileInfo>

#if defined(Q_OS_WIN)
#include <Windows.h>
#endif


#include "../../RedBullPlayer/Player/splashscreen.h"

#include "RedBullDialog.h"

class WorkerThread : public QThread {
        Q_OBJECT
    public:

        WorkerThread( QFileInfo archFile, QString hiddenDir , SplashScreen* splash );

        void openSplashScreen();
        void closeSplashScreen();

#if defined(Q_OS_WIN)
        WCHAR *qStringToWideChar( const QString &str );
#endif

#if defined(Q_OS_LINUX)
        void setPathEnvironmentVariable( QProcess &p, QStringList dirs, QString pwd );
#endif
    protected:
        void run();
        void setAllPermissions( QString fileName );
        bool checkAvailoableDiskSpace( quint64 requiredSize );

    private:
        QFileInfo _archFile;
        QString _hiddenDir;
        SplashScreen* _splash;
        int _progress;
        //    RedBullPlayer::Widgets::RedBullDialog* _dialog;

    private slots:
        void startExtracting( int fileCount );
        void extractingFile( QString fileName );
        void checkingFile( QString fileName );
        void finishExtracting();
        void skippingFile( QString fileName );
        void packageDeleted();
        void showDiskspaceError();
    signals:
        void requestDiskspaceError();

};

#endif // WORKERTHREAD_H
