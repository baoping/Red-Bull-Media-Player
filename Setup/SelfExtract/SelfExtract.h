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
#ifndef SELFEXTRACT_H
#define SELFEXTRACT_H

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <QProcess>

#if defined(Q_OS_WIN)
#include <Windows.h>
#endif

//static SplashScreen* splash;
void CreateHiddenDirectory( const char* name );
void openSplashScreen();
void closeSplashScreen();

void ReursiveDeleteDirectory( const QString d );

#if defined(Q_OS_WIN)
WCHAR *qStringToWideChar( const QString &str );
#endif

#if defined(Q_OS_LINUX)
void setPathEnvironmentVariable( QProcess &p, QStringList dirs, QString pwd );
#endif

#endif // SELFEXTRACT_H
