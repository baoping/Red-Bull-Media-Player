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
#ifndef ERROR_H
#define ERROR_H

#include <QString>

#include "IError.h"

namespace RedBullPlayer {
    namespace Container {
        class Error : public RedBullPlayer::IError {
            public:
                Error();

                virtual void setErrorMessage( const QString& errorMessage, RedBullPlayer::IError::ErrorType errorType = RedBullPlayer::IError::Normal );

                virtual const QString& errorMessage() const {
                    return _errorMessage;
                }

                virtual const RedBullPlayer::IError::ErrorType& errorType() const {
                    return _errorType;
                }

                virtual void reset();
                static bool raise( RedBullPlayer::IError* error, bool reset = true );

            private:
                QString _errorMessage;
                RedBullPlayer::IError::ErrorType _errorType;
        };
    }
}
#endif // ERROR_H
