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
#include "TextBrowser.h"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

#include "../ToolLib/ConfigManager.h"
#include "../Interfaces/MediaLibrary/INewsImage.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::MediaLibrary;

TextBrowser::TextBrowser( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QWidget* parent ) : QTextBrowser( parent ) {
    _mediaLibrary = mediaLibrary;
    setOpenExternalLinks( true );
    setOpenLinks( true );
    setStyleSheet("background:rgb(0,123,189);");
}

QVariant TextBrowser::loadResource( int type, const QUrl& name ) {
    if ( type == QTextDocument::ImageResource ) {
        INewsImage* newsImage = _mediaLibrary->mediaDB()->newsRepository()->getNewsImageByFileName( _newsGuid, QFileInfo( name.path() ).fileName() );

        if ( newsImage != 0 ) {
            return newsImage->image();
        }
    }

    return QTextBrowser::loadResource( type, name );
}
