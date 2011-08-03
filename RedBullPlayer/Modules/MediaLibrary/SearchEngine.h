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
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H


// Qt includes
#include <QObject>
#include <QUuid>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QList>

// Projectincludes
#include "MediaDB.h"
#include "../../Interfaces/MediaLibrary/ISearchEngine.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Container/Error.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class SearchEngine : public ISearchEngine {
                    Q_OBJECT

                public:
                    SearchEngine( IMediaLibrary* mediaLibrary,  QObject* parent );
                    virtual void queryForName( QString query );

                private:
                    QList<QUuid>* doQuickDBSearch( QString query );
                    void doSlowgQuery( QString query, QList<QUuid>* allreadyFoundItems );
                    bool isNameMatch( IMedia* media, QStringList searchTerms );
                    IMediaLibrary* _mediaLibrary;

            };
        }
    }
}

#endif // SEARCHENGINE_H
