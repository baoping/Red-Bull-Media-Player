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
#ifndef ADDSCANEDDIRECTORYSETTINGVIEW_H
#define ADDSCANEDDIRECTORYSETTINGVIEW_H

#include <QtGui/QWidget>
#include <QUuid>

#include "../../Interfaces/UserSettings/ISettingsAddDirectorySettingView.h"
#include "../../Widgets/CommandButton.h"

namespace Ui {
    class AddScanedDirectorySettingView;
}

class AddScanedDirectorySettingView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsAddDirectorySettingView {
        Q_OBJECT
    public:
        AddScanedDirectorySettingView( QWidget *parent, QUuid guid );
        ~AddScanedDirectorySettingView();

        virtual QUuid guid() const;

    protected:
        void changeEvent( QEvent *e );

    private:
        Ui::AddScanedDirectorySettingView *_ui;
        QUuid _guid;
        RedBullPlayer::Widgets::CommandButton *_addButton;
};

#endif // ADDSCANEDDIRECTORYSETTINGVIEW_H
