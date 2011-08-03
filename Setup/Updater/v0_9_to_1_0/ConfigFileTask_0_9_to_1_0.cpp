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
#include "ConfigFileTask_0_9_to_1_0.h"


#include <QMessageBox>

using namespace RedBullPlayer::Updater;


ConfigFileTask_0_9_to_1_0::ConfigFileTask_0_9_to_1_0(
#if defined(Q_OS_MAC)
    QString oldAppBudleName
#else
    QString updateFileName
#endif
) : ConfigFileTask(
#if defined(Q_OS_MAC)
        oldAppBudleName,
#else
        updateFileName,
#endif
        QString( "Update Configurration File from v0.9 -> v1.0" )
    ) {
}


void ConfigFileTask_0_9_to_1_0::updateSettings() {
    // QMessageBox::information(0, "Update Configurration File from v0.9 -> v1.0","Update Configurration File from v0.9 -> v1.0" );
    _playerSettings->setValue( "LastUpdateStep", "0.9.x to 1.0.x" );
}

