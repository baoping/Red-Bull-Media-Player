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
#include "LoopPlaylistSettingModel.h"

#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Tools;

LoopPlaylistSettingModel::LoopPlaylistSettingModel( QObject *parent, ISettingsBooleanSettingView *view )
    : ISettingsModel( parent ) {
    _view = view;
    _currentSettingValue = ConfigManager::instance()->loopPlaylist();
    _view->setValue( _currentSettingValue );
    connect( _view, SIGNAL( valueChanged( bool ) ), this, SLOT( valueChanged( bool ) ) );
}

void LoopPlaylistSettingModel::valueChanged( bool value ) {
    _currentSettingValue = value;
}

void LoopPlaylistSettingModel::saveData() {
    ConfigManager::instance()->setLoopPlaylist( _currentSettingValue );
}
