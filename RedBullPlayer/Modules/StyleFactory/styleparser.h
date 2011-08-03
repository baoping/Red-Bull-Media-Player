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
#ifndef STYLEPARSER_H
#define STYLEPARSER_H

//Qt includes
#include <QObject>
#include <QMap>
#include <QFileInfo>
#include <QXmlStreamReader>

//Project includes
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StyleFactory {
            class StyleParser : public QObject {
                    Q_OBJECT
                public:
                    StyleParser( QObject *parent = 0 );
                    QMap<QString, ITextStyle*>* parseConfigFile( QFileInfo configFile );
                private:
                    ITextStyle* parseTextStyleElement( QXmlStreamReader* xml );
            };
        }
    }
}

#endif // STYLEPARSER_H
