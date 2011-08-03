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
#ifndef PLAYERCLOSEBUTTONUI_H
#define PLAYERCLOSEBUTTONUI_H

#include <QtGui/QWidget>

#include "../../PlayerShell/Shell.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class PlayerCloseButtonUI;
}

class PlayerCloseButtonUI : public QWidget {
        Q_OBJECT
    public:
        PlayerCloseButtonUI( RedBullPlayer::PlayerShell::Shell *shell, QWidget *parent = 0 );
        ~PlayerCloseButtonUI();

    signals:
        void playerCloseButtonClicked();

    public slots:
        void playerCloseButtonClickedSlot();

    protected:
        void changeEvent( QEvent *e );

    private:
        Ui::PlayerCloseButtonUI *_ui;
        RedBullPlayer::Widgets::CommandButton *_closeButton;

};

#endif // PLAYERCLOSEBUTTONUI_H
