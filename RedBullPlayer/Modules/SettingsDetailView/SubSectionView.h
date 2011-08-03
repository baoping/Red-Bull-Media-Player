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
#ifndef SUBSECTIONVIEW_H
#define SUBSECTIONVIEW_H

#include <QtGui/QWidget>

#include "../../Interfaces/UserSettings/ISettingsSubSectionView.h"

namespace Ui {
    class SubSectionView;
}

class SubSectionView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsSubSectionView {
        Q_OBJECT
    public:
        SubSectionView( QWidget *parent, QString title );
        ~SubSectionView();

        virtual void addSettingView( RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *settingView );
        virtual void insertSettingView( int atIndex, RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *settingView );
        virtual void removeSettingView( RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *settingView );
        virtual void setTitleStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *titleStyle );

    protected:
        void changeEvent( QEvent *e );

    private:
        Ui::SubSectionView *_ui;
};

#endif // SUBSECTIONVIEW_H
