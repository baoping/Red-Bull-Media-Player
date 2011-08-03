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
#ifndef FILEEXTENSIONSSETTINGVIEW_H
#define FILEEXTENSIONSSETTINGVIEW_H

#include <QtGui/QWidget>
#include <QUuid>
#include <QKeyEvent>

#include "../../Interfaces/UserSettings/ISettingsFileExtensionsSettingView.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class FileExtensionsSettingView;
}

class FileExtensionsSettingView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsFileExtensionsSettingView {
        Q_OBJECT
    public:
        FileExtensionsSettingView( QWidget *parent, QUuid guid, QString labelText );
        ~FileExtensionsSettingView();

        void setText( QString text );
        virtual QUuid guid() const;
        virtual void setLabelStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *labelStyle );
        virtual void setTextBoxStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textBoxStyle );
        virtual void setValue( QString value );
        virtual QString getValue();

    protected:
        void changeEvent( QEvent *e );
        void keyReleaseEvent( QKeyEvent *e );

    private:
        Ui::FileExtensionsSettingView *_ui;
        QUuid _guid;

};

#endif // FILEEXTENSIONSSETTINGVIEW_H
