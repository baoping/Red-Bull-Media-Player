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
#ifndef UPDATEENGINE_H
#define UPDATEENGINE_H

#include <QObject>
#include <QThread>

#include "../Widgets/UpdateProgressWidget.h"
#include "UpdateManager.h"
#include "UpdateProcess.h"

namespace RedBullPlayer {
    namespace Updater {
        class UpdateEngine : public QThread {
                Q_OBJECT
            public:
                UpdateEngine( QObject* parent );
                void prepareExecute(
                    RedBullPlayer::Widgets::UpdateProgressWidget* progressWidget,
                    QString fromVersion,
                    QString toVersion,
                    QString updateDataFile,
                    QString originalPackageFileName );
            protected:
                virtual void run();

            signals:
                void updateMessage( QString message );
            private:
                void setupCommonPostUpdateTasks();
                void setupCommonPreUpdateTasks();
                void setupCustomUpdateTasks();
                void runTasks();
                int taskCount();
            private:
                RedBullPlayer::Widgets::UpdateProgressWidget *_progressWidget;
                UpdateManager* _updateManager;
                UpdateProcess* _commonPostUpdate;
                UpdateProcess* _commonPreUpdate;
                QString _fromVersion;
                QString _toVersion;
                QString _updateFileName;
                QString _originalPackageFileName;
        };

    }
}
#endif // UPDATEENGINE_H
