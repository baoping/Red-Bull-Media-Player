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
#ifndef SCANEDDIRECTORYSETTINGVIEW_H
#define SCANEDDIRECTORYSETTINGVIEW_H

#include <QtGui/QWidget>
#include <QUuid>
#include <QKeyEvent>

#include "../../Interfaces/UserSettings/ISettingsScanDirectorySettingView.h"
#include "../../Widgets/CommandButton.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class ScanedDirectorySettingView;
}

class ScanedDirectorySettingView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsScanDirectorySettingView {
        Q_OBJECT
    public:
        ScanedDirectorySettingView( QWidget *parent, QUuid guid );
        ~ScanedDirectorySettingView();

        virtual QUuid guid() const;
        virtual void setTextBoxStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textBoxStyle );
        virtual void setValue( QString value );
        virtual QString getValue();
        virtual void setReadOnly( bool readOnly );

    protected slots:
        void removedClickedSlot();

    protected:
        void changeEvent( QEvent *e );
        void keyReleaseEvent ( QKeyEvent * e );

    private:
        Ui::ScanedDirectorySettingView *_ui;
        QUuid _guid;
        RedBullPlayer::Widgets::CommandButton *_addButton;
        RedBullPlayer::Widgets::CommandButton *_removeButton;
};

#endif // SCANEDDIRECTORYSETTINGVIEW_H
