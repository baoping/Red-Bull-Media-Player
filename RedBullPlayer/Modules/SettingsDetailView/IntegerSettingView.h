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
#ifndef INTEGERSETTINGVIEW_H
#define INTEGERSETTINGVIEW_H

#include <QtGui/QWidget>
#include <QUuid>
#include <QKeyEvent>

#include "../../Interfaces/UserSettings/ISettingsIntegerSettingView.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class IntegerSettingView;
}

class IntegerSettingView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsIntegerSettingView {
        Q_OBJECT
    public:
        IntegerSettingView( QWidget *parent, QUuid guid, QString text );
        ~IntegerSettingView();

        virtual QUuid guid() const;
        virtual void setLabelStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *labelStyle );
        virtual void setValue( int value );
        virtual int getValue();

    protected:
        void changeEvent( QEvent *e );
        void keyReleaseEvent( QKeyEvent *e );

    private:
        Ui::IntegerSettingView *_ui;
        QUuid _guid;
};

#endif // INTEGERSETTINGVIEW_H
