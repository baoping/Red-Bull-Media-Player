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
#ifndef CONTENTCATEGORYENUM_H
#define CONTENTCATEGORYENUM_H

#include <QMetaType>

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {

            class ContentCategory : QObject {
                    Q_OBJECT
                    Q_ENUMS( CategoryEnum )

                public:
                    enum CategoryEnum {
                        NEW = 0,
                        DOWNLOADED = 1,
                        SKIPPED = 2
                    };

                    explicit ContentCategory( CategoryEnum category ) : QObject( 0 ) {
                        _category = category;
                    }

                    CategoryEnum value() const {
                        return _category;
                    }


                    static QString categoryString( CategoryEnum category ) {
                        QString categoryStr = "";

                        switch( category ) {
                            case ContentCategory::NEW:
                                categoryStr = "NEW";
                                break;

                            case ContentCategory::DOWNLOADED:
                                categoryStr = "DOWNLOADED";
                                break;

                            case ContentCategory::SKIPPED:
                                categoryStr = "SKIPPED";
                                break;
                        }

                        return categoryStr;
                    }
                private:
                    CategoryEnum _category;

            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum );

#endif // CONTENTCATEGORYENUM_H
