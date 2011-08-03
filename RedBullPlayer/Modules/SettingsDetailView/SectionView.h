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
#ifndef SECTIONVIEW_H
#define SECTIONVIEW_H

#include <QtGui/QWidget>

#include "../../Interfaces/UserSettings/ISettingsSectionView.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace Ui {
    class SectionView;
}

class SectionView : public RedBullPlayer::Modules::SettingsDetailView::ISettingsSectionView {
        Q_OBJECT
    public:
        SectionView( QString title, QString help, QWidget *parent = 0 );
        ~SectionView();

        virtual void addSubSectionView( RedBullPlayer::Modules::SettingsDetailView::ISettingsSubSectionView *subSectionView );
        virtual void setTitleStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *titleStyle );

    protected:
        void changeEvent( QEvent *e );


    private:
        Ui::SectionView *_ui;
};

#endif // SECTIONVIEW_H
