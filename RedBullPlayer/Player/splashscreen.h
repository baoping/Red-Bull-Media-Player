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
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QtGui/QFrame>

#include "../Widgets/CircleProgress.h"

namespace Ui {
    class SplashScreen;
}

class SplashScreen : public QFrame {
        Q_OBJECT
    public:
        SplashScreen( QWidget *parent = 0 );
        ~SplashScreen();

        RedBullPlayer::Widgets::CircleProgress* progressWidget() const {
            return _progress;
        }

        void setWindowSize();

    protected:
        void changeEvent( QEvent *e );

    private:
        Ui::SplashScreen *m_ui;

        RedBullPlayer::Widgets::CircleProgress* _progress;

    public slots:
        void initializeProgress( double minimum, double maximum, double step = 0 );
        void progress( double position, QString text = "" );
        void doStep();
        void setProgressText( QString text );
};

#endif // SPLASHSCREEN_H
