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
#ifndef SNAPSHOTCREATORMODULE_H
#define SNAPSHOTCREATORMODULE_H

//QT Includes
#include <QObject>
#include <QEvent>
#include <QTimer>
#include <QQueue>
#include <QMutex>

// Project Includes
#include "../../Interfaces/ISnapshotCreator.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerExitClickedEvent.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestedEvent.h"
#include "../../Events/SnapshotCreator/CancelSnapshotCreationEvent.h"
#include "../../Container/Error.h"
#include "SnapshotRequestHandler.h"
#include "RequestMetaData.h"



namespace RedBullPlayer {
    namespace Modules {
        namespace SnapshotCreator {
            class SnapshotCreatorModule : public ISnapshotCreator {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::SnapshotCreator::ISnapshotCreator )

                public:
                    SnapshotCreatorModule();
                    virtual ~SnapshotCreatorModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual void cleanUp();
                    virtual QString name() const {
                        return "Playlist";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    virtual bool event( QEvent* );
                private: //functions
                    void onCloseButtonClicked( RedBullPlayer::Events::PlayerExitClickedEvent* ev );
                    void onSnapshotCreationRequested( RedBullPlayer::Events::SnapshotCreationRequestedEvent* ev );
                    void onCancelSnapshotCreation( RedBullPlayer::Events::CancelSnapshotCreationEvent* ev );

                    void clearQueue();
                private: //data
                    QTimer* _checkTimer;
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    SnapshotRequestHandler* _requestHandler;
                    QQueue<RequestMetaData*> _requestQueue;

                    mutable QMutex _checkMutex;
                    mutable QMutex _clearMutex;
                    mutable QMutex _requestMutex;
                    mutable QMutex _cancelMutex;
                    mutable QMutex _finishedMutex;
                    mutable QMutex _failedMutex;

                private slots:
                    void snapshotFinished( QUuid id, QString fileName );
                    void snapshotFailed( QUuid id );
                    void onCheckTimer();
            };
        }
    }
}

#endif // SNAPSHOTCREATORMODULE_H
