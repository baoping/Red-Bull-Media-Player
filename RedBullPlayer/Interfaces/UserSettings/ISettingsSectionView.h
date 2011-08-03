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
#ifndef ISETTINGSSECTIONVIEW_H
#define ISETTINGSSECTIONVIEW_H

#include <QObject>
#include <QWidget>

#include "ISettingsSubSectionView.h"
#include "../StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISettingsSectionView : public QWidget {
                    Q_OBJECT

                public:
                    ISettingsSectionView( QWidget* parent = 0 ) : QWidget( parent ) { }
                    virtual ~ISettingsSectionView() {}

                    virtual void addSubSectionView( RedBullPlayer::Modules::SettingsDetailView::ISettingsSubSectionView *subSectionView ) = 0;
                    virtual  void setTitleStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *titleStyle ) = 0;
            };
        }
    }
}

#endif // ISETTINGSSECTIONVIEW_H
