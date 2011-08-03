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
#include "CollectStatisticsSettingModel.h"


using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::MediaLibrary;

CollectStatisticsSettingModel::CollectStatisticsSettingModel( QObject *parent, ISettingsBooleanSettingView *view,
        IMediaLibrary *mediaLibrary ) : ISettingsModel( parent ) {
    _view = view;
    _mediaLibrary = mediaLibrary;
    _originalValue = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowCollectStatistic();
    _currentSettingValue = _originalValue;
    _view->setValue( _originalValue );
    connect( _view, SIGNAL( valueChanged( bool ) ), this, SLOT( valueChanged( bool ) ) );
}

void CollectStatisticsSettingModel::valueChanged( bool value ) {
    _currentSettingValue = value;
}

void CollectStatisticsSettingModel::saveData() {
    if( _originalValue != _currentSettingValue ) {
        _mediaLibrary->mediaDB()->systemMetaDataRepository()->setAllowCollectStatistic( _currentSettingValue );
        _originalValue = _currentSettingValue;
        emit originalValueChanged();
    }
}



