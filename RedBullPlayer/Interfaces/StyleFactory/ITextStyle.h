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
#ifndef ITEXTSTYLE_H
#define ITEXTSTYLE_H

#include <QString>
#include <QUuid>
#include <QColor>

namespace RedBullPlayer {
    namespace Modules {
        namespace StyleFactory {
            class ITextStyle  {
                public:
                    virtual ~ITextStyle() {}
                    virtual QString name() const = 0;
                    virtual QString fontName() const = 0;
                    virtual int pointSize() const = 0;
                    virtual bool bold() const = 0;
                    virtual bool italic() const = 0;
                    virtual QColor foreColor() const = 0;
                    virtual QString rgbaColorString() const = 0;
                    virtual Qt::Alignment alignment() const = 0;
                    virtual QString styleSheet() const = 0;
            };
        }
    }
}

#endif // ITEXTSTYLE_H
