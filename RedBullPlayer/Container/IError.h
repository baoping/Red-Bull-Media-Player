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
#ifndef IERROR_H
#define IERROR_H

#include <QString>

namespace RedBullPlayer {
    class IError {
        public:
            enum ErrorType {
                NoError = 1,
                Warning = 2,
                Normal = 4,
                Fatal = 8
            };

            virtual ~IError() {}

            virtual void setErrorMessage( const QString& errorMessage, IError::ErrorType errorType = IError::Normal ) = 0;
            virtual const QString& errorMessage() const = 0;
            virtual const IError::ErrorType& errorType() const = 0;
            virtual void reset() = 0;

    };
}



#endif // IERROR_H
