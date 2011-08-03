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
#ifndef ISEARCHENGINE_H
#define ISEARCHENGINE_H

#include <QList>
#include <QObject>

#include "IMediaType.h"
#include "ITagRepository.h"
#include "IMediaRepository.h"
#include "ISnapshotRepository.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class ISearchEngine : public QObject {
                    Q_OBJECT
                public:
                    ISearchEngine( QObject* parent ) : QObject( parent ) {}
                    virtual ~ISearchEngine() {}
                    virtual void queryForName( QString query ) = 0;
                signals:
                    void mediaFound( QString mediaID );
            };
        }
    }
}
#endif // ISEARCHENGINE_H
