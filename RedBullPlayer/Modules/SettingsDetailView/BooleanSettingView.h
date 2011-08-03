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
#ifndef BOOLEANSETTINGVIEW_H
#define BOOLEANSETTINGVIEW_H

#include <QtGui/QWidget>
#include <QUuid>

#include "../../Interfaces/UserSettings/ISettingsBooleanSettingView.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class BooleanSettingView;
}

class BooleanSettingView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsBooleanSettingView {
        Q_OBJECT
    public:
        BooleanSettingView( QWidget *parent, QUuid guid, QString text );
        ~BooleanSettingView();

        virtual QUuid guid() const;
        virtual void setLabelStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *labelStyle );
        virtual void setValue( bool value );

    protected:
        void changeEvent( QEvent *e );

        void styleCheckBox();

    private:
        Ui::BooleanSettingView *_ui;
        QUuid _guid;
};

#endif // BOOLEANSETTINGVIEW_H
