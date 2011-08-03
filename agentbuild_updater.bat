@echo off

set QT_INSTALL_DIR=%1

:: %QT_INSTALL_DIR%\bin\qtenv.bat
:: echo -- QTDIR set to %QTDIR%
::

set QTDIR=%QT_INSTALL_DIR%\qt
set PATH=%QT_INSTALL_DIR%\bin
set PATH=%PATH%;%QT_INSTALL_DIR%\bin;%QT_INSTALL_DIR%\mingw\bin
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++

:: echo %PATH%

rmdir /Q /S bin\
rmdir /Q /S obj\

cd Setup\Updater
%QT_INSTALL_DIR%\qt\bin\qmake.exe -spec win32-g++
 :: || exit 1
 %QT_INSTALL_DIR%\mingw\bin\mingw32-make.exe &
:: || exit 1
cd ..\..\

