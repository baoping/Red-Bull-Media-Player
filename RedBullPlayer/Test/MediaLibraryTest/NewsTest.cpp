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
#include "NewsTest.h"

#include <QDebug>

#include "News/News.h"
#include "News/NewsImage.h"
#include "News/NewsRepository.h"
#include "MediaLibrary/IMediaDB.h"
#include "MediaDB.h"
#include "Shell.h"
#include "Error.h"
#include "UuidCreator.h"
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;


void NewsTest::should_validate_news() {
    INews* news = new News( RedBullPlayer::Tools::UuidCreator::create(), "TestNews" );
    QVERIFY2( news->isValid(), "News Item must be valid" );
}

void NewsTest::should_invalidate_news_string_empty() {
    INews* news2 = new News( RedBullPlayer::Tools::UuidCreator::create(), "" );
    QVERIFY2( ! news2->isValid(), "News Item must not be valid" );
}

void NewsTest::should_invalidate_news_string_null() {
    INews* news3 = new News( RedBullPlayer::Tools::UuidCreator::create(), QString::null );
    QVERIFY2( ! news3->isValid(), "News Item must not be valid" );
}
