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
#include "MediaLibraryModuleTest.h"

#include <QDomElement>
#include <QDomDocument>
#include <QString>


#include "MediaLibraryModule.h"
#include "MediaLibrary/IMediaDB.h"
#include "MediaLibrary/IMediaMetadataHandler.h"
#include "MediaLibrary/ISearchEngine.h"
#include "MediaLibrary/IMediaDB.h"


#include "../../PlayerShell/Shell.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;

void MediaLibraryModuleTest::should_initialize_with_null() {
    new Shell( 0 );
    MediaLibraryModule* mod = new MediaLibraryModule();
    QVERIFY( mod->mediaDB() == 0 );
    QVERIFY( mod->metadataHandler() == 0 );
    QVERIFY( mod->searchEngine() == 0 );
}


void MediaLibraryModuleTest::Setup_InitDB() {
    Shell* s = new Shell( 0 );
    MediaLibraryModule* mod = new MediaLibraryModule();
    QDomDocument doc;
    doc.setContent( QString( "<ModuleConfig instanceName=\"DefaultInstance\" />" ) );
    mod->Setup( s, doc.documentElement() );
    QVERIFY( mod->mediaDB() != 0 );
    QVERIFY( dynamic_cast<IMediaDB*> ( mod->mediaDB() ) );
    QVERIFY( mod->metadataHandler() != 0 );
    QVERIFY( dynamic_cast<IMediaMetadataHandler*> ( mod->metadataHandler() ) );
    QVERIFY( mod->searchEngine() != 0 );
    QVERIFY( dynamic_cast<ISearchEngine*> ( mod->searchEngine() ) );
    mod->deleteLater();
}


