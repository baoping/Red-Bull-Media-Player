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
#ifndef STYLEFACTORYMODULE_H
#define STYLEFACTORYMODULE_H

//QT Includes
#include <QObject>
#include <QMap>

// Project Includes
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../PlayerShell/Shell.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StyleFactory {
            class StyleFactoryModule : public IStyleFactory {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::StyleFactory::IStyleFactory )

                public:
                    StyleFactoryModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "StyleFactory";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                    virtual ITextStyle* getTextStyle( QString name, bool *ok );
                protected:
                    bool event( QEvent * e );
                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    QMap<QString, ITextStyle*>* _styles;

                    void bootstrap();
            };
        }
    }
}

#endif // STYLEFACTORYMODULE_H
