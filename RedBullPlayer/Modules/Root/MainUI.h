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
#ifndef MAINUI_H
#define MAINUI_H

#include <QtGui/QWidget>
#include <QPixmap>

#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"

using namespace RedBullPlayer::PlayerShell;

namespace Ui {
    class MainUI;
}

class MainUI : public QWidget {
        Q_OBJECT
    public:
        MainUI( Shell* shell, QWidget *parent = 0 );
        ~MainUI();


    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );

    private:
        Ui::MainUI *m_ui;
        Shell* _shell;

        QPixmap _backgroundImage;
        QPixmap _horizontalGradientLine;
        QPixmap _verticalGradientLine;
};

#endif // MAINUI_H
