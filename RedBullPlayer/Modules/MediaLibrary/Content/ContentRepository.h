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
#ifndef CONTENTREPOSITORY_H
#define CONTENTREPOSITORY_H

// Qt includes
#include <QObject>
#include <QUuid>
#include <QString>
#include <QMutex>
#include <QtSql>
#include <QSqlDatabase>

// Project includes
#include "../../../Interfaces/MediaLibrary/IContentRepository.h"
#include "../../../Container/Error.h"
#include "../MediaDB.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class ContentRepository : public QObject, public IContentRepository {
                    Q_OBJECT

                public:
                    ContentRepository( MediaDB* mediaDb, QObject* parent );
                    virtual ~ContentRepository();

                    virtual IContent* createContent( const QString& guid,
                                                     const QString& name,
                                                     const QString& fileName,
                                                     const QString& downloadUrl );
                    virtual bool saveContent( const IContent& content, RedBullPlayer::Container::Error* error );
                    virtual IContent* getContentByGuid( QString guid );
                    virtual IContent* getContentByMediaGuid( QString mediaGuid );
                    virtual QList<IContent*>* allContent();
                    virtual QList<IContent*>* allContentByState( QString state );
                    virtual bool deleteContent( const IContent& content, RedBullPlayer::Container::Error* error );

                private: //Methods
                    IContent* createContentFromSqlResult( QSqlRecord record );

                private: // Data
                    MediaDB* _mediaDb;
                    mutable QMutex _createContentMutex;
                    mutable QMutex _saveContentMutex;
                    mutable QMutex _getContentByGuidMutex;
                    mutable QMutex _getContentByMediaGuidMutex;
                    mutable QMutex _allContentMutex;
                    mutable QMutex _allContentByStateMutex;
                    mutable QMutex _deleteContentMutex;
                    mutable QMutex _createContentFromSqlResultMutex;
            };
        }
    }
}

#endif // CONTENTREPOSITORY_H
