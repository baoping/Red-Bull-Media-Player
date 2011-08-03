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
#ifndef ISETTINGSINTEGERSETTINGVIEW_H
#define ISETTINGSINTEGERSETTINGVIEW_H

#include "ISettingsSettingView.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISettingsIntegerSettingView : public ISettingsSettingView {
                    Q_OBJECT

                public:
                    ISettingsIntegerSettingView( QWidget *parent = 0 ) : ISettingsSettingView( parent ) {}
                    ~ISettingsIntegerSettingView() {}

                    virtual void setValue( int vlaue ) = 0;
                    virtual int getValue() = 0;
            };
        }
    }
}
#endif // ISETTINGSINTEGERSETTINGVIEW_H
