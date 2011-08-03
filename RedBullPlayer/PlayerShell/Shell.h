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
#ifndef SHELL_H
#define SHELL_H

// QT Includes
#include <QtGui/QWidget>
#include <QList>
#include <QCloseEvent>

// Project Includes
#include "UI/Region.h"
#include "../Container/Error.h"
#include "../Container/Container.h"
#include "../Widgets/CircleProgress.h"

namespace Ui {
    class Shell;
}

namespace RedBullPlayer {
    namespace PlayerShell {
        class Shell : public QWidget {
                Q_OBJECT

            public:
                Shell( QWidget *parent = 0 );
                ~Shell();
                void setProgressWidget( RedBullPlayer::Widgets::CircleProgress* circleProgress );
                void Bootstrap();
                void registerRegion( RedBullPlayer::PlayerShell::Region* region );

                QList<RedBullPlayer::PlayerShell::Region*> regions() const {
                    return _regions;
                }

                RedBullPlayer::PlayerShell::Region* findRegion( QString name );
                RedBullPlayer::PlayerShell::Region* RootRegion();
                RedBullPlayer::Container::Error* error() {
                    return _error;
                }
                void invokeKeyReleaseEvent( QKeyEvent * event );
                void toggleSize();
                void toggleFullscreen();
                void setWindowSize();

                void resize( const QSize newSize );
                void move( const QPoint newPoint );
                void switchToNormalScreenForResize();
                QString installerStubAbsolutePath();

            protected:
                void setupSplashScreenConnections();
                void moveEvent( QMoveEvent *e );
                void keyReleaseEvent ( QKeyEvent * event );
                void resizeToNormal();
                void resizeToMaximum();

            private slots:
                void startLoading( int pluginCount );
                void loadingPlugin( QString pluginName );
                void initializingPlugin( QString pluginName );
            private:
                Ui::Shell *ui;
                QList<RedBullPlayer::PlayerShell::Region*> _regions;
                RedBullPlayer::PlayerShell::Region* _rootRegion;
                RedBullPlayer::Container::Error* _error;
                RedBullPlayer::Widgets::CircleProgress* _circleProgress;
                QSize _oldSize;
                QRect _oldGeometry;
                bool _isNormalSize;
                int _lodingProgress;
        };
    }
}
#endif // SHELL_H
