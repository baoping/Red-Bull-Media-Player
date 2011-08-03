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
#include "ContentDownloadSettingModel.h"

#include <QDebug>

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::MediaLibrary;

ContentDownloadSettingModel::ContentDownloadSettingModel( QObject *parent, ISettingsBooleanSettingView *view,
        IMediaLibrary *mediaLibrary ) : ISettingsModel( parent ) {
    _view = view;
    _mediaLibrary = mediaLibrary;
    _originalValue = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    _currentSettingValue = _originalValue;
    qDebug() << "ContentDownloadSettingModel => AllowNetworkConnection=" << _originalValue;
    _view->setValue( _originalValue );
    connect( _view, SIGNAL( valueChanged( bool ) ), this, SLOT( valueChanged( bool ) ) );
}

void ContentDownloadSettingModel::valueChanged( bool value ) {
    qDebug() << "ContentDownloadSettingModel => AllowNetworkConnection=" << value;
    _currentSettingValue = value;
}

void ContentDownloadSettingModel::saveData() {
    if( _originalValue != _currentSettingValue ) {
        qDebug() << "ContentDownloadSettingModel => Save AllowNetworkConnection=" << _currentSettingValue;
        _mediaLibrary->mediaDB()->systemMetaDataRepository()->setAllowNetworkConnection( _currentSettingValue );
        _originalValue = _currentSettingValue;
        emit originalValueChanged();
    }
}
