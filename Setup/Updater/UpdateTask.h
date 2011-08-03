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
#ifndef UPDATETASK_H
#define UPDATETASK_H

//QT Includes
#include <QObject>
#include <QString>
#include <QCoreApplication>

namespace RedBullPlayer {
    namespace Updater {
        class UpdateTask : public QObject {
                Q_OBJECT
            public:
                UpdateTask( QString name, QObject* parent = 0 ) : QObject( parent ) {
                    _name = name;
                }
                virtual ~UpdateTask() {}
                virtual void execute() {
                    emit taskStarted( name() );
                    qApp->processEvents();

                    if ( doWork() ) {
                        emit taskFinished( name() );
                        qApp->processEvents();
                    }    else {
                        emit taskFailed( name(), error() );
                        qApp->processEvents();
                    }
                }
                virtual bool doWork() = 0;
                QString name() const {
                    return _name;
                }
                QString error() const {
                    return _error;
                }

                void setError( QString error ) {
                    _error = error;
                }

            signals:
                void taskStarted( QString name );
                void taskFinished( QString name );
                void taskFailed( QString name, QString errorMsg );
            private:
                QString _name;
                QString _error;

        };
    }
}


#endif // UPDATETASK_H
