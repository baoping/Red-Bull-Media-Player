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
#include "Error.h"

// Qt includes
#include <QObject>
#include <QDebug>
#include <QMessageBox>

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer;

Error::Error() {
    _errorType = Error::NoError;
}

void Error::setErrorMessage( const QString& errorMessage,  RedBullPlayer::IError::ErrorType errorType ) {
    _errorMessage = errorMessage;
    _errorType = errorType;
}

void Error::reset() {
    _errorMessage = QString::null;
    _errorType = IError::NoError;
}

// Static stuff
bool Error::raise( IError* error, bool reset /* = true */ ) {
    switch( error->errorType() ) {
        case IError::NoError:
            return true;

        case IError::Warning:
            QMessageBox::warning( 0, "Warning", error->errorMessage() );

            if ( reset ) {
                error->reset();
            }

            qWarning() << "Warning: " << error->errorMessage();
            return true;

        case IError::Normal:
            QMessageBox::critical( 0, "Error", error->errorMessage() );

            if ( reset ) {
                error->reset();
            }

            qWarning() << "Error: " << error->errorMessage();
            return false;

        case IError::Fatal:
            QMessageBox::critical( 0, "Fatal Error", error->errorMessage() );

            if ( reset ) {
                error->reset();
            }

            qFatal( error->errorMessage().toLatin1() );
            return false;

        default:
            qFatal( "Invalid ErrorType value!" );
            return false;
    }
}
