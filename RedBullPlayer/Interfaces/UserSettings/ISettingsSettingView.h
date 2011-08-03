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
#ifndef ISETTINGSSETTINGVIEW_H
#define ISETTINGSSETTINGVIEW_H

#include <QObject>
#include <QWidget>
#include <QUuid>

#include "../StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISettingsSettingView : public QWidget {
                    Q_OBJECT

                public:
                    ISettingsSettingView( QWidget* parent = 0 ) : QWidget( parent ) { }
                    virtual ~ISettingsSettingView() {}

                    virtual QUuid guid() const = 0;
                    virtual void setLabelStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *labelStyle ) {}
                    virtual void setTextBoxStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textBoxStyle ) {}
            };
        }
    }
}

#endif // ISETTINGSSETTINGVIEW_H
