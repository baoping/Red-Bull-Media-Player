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
#include "rssfeedtest.h"

// QT includes
#include <QFileInfo>
#include <QMap>
#include <QFile>

#include "RssFeed.h"
#include "IRssFeed.h"
#include "RssFeedItem.h"
#include "IRssFeedItem.h"
#include "RssFeedImageDescriptor.h"
#include "IRssFeedImageDescriptor.h"


RssFeedTest::RssFeedTest() {
}

void RssFeedTest::test_file_should_exists() {
    QFileInfo fileInfo( "testRssFeed.xml" );
    QVERIFY( QFile::exists( fileInfo.absoluteFilePath() ) );
}

void RssFeedTest::should_have_any_feedItems() {
    QFileInfo fileInfo( "testRssFeed.xml" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadOnly ) );
    RedBullPlayer::Tools::RSS::RssFeed rssFeed( file.readAll() );
    file.close();
    QVERIFY( rssFeed.itemCount() > 0 );
}

void RssFeedTest::should_have_3_feedItems() {
    QFileInfo fileInfo( "testRssFeed.xml" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadOnly ) );
    RedBullPlayer::Tools::RSS::RssFeed rssFeed( file.readAll() );
    file.close();
    QVERIFY( rssFeed.itemCount() == 3 );
}

void RssFeedTest::should_have_defined_values() {
    QFileInfo fileInfo( "testRssFeed.xml" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadOnly ) );
    RedBullPlayer::Tools::RSS::RssFeed rssFeed( file.readAll() );
    file.close();
    QVERIFY( rssFeed.title() == "Title of my PodCast" );
    QVERIFY( rssFeed.description() == "Description of my PodCast" );
    QVERIFY( rssFeed.link() == "URL to my PodCast's Homepage" );
    QVERIFY( rssFeed.language() == "de-at" );
    QVERIFY( rssFeed.copyright() == "Any Copyright Things" );
    QVERIFY( rssFeed.pubDate() == RedBullPlayer::Tools::RSS::RssFeed::dateTimeFromRFC822( "Mon, 22 Feb 2010 10:30:00 +0100" ) );
    QVERIFY( rssFeed.webMaster() == "Webmaster's Email Adress" );
    /* missing
       - lastBuildDate
       - docs
       - managingEditor
       - category
       - generator
       - rating
       */
}

void RssFeedTest::should_have_items_and_defined_values() {
    QFileInfo fileInfo( "testRssFeed.xml" );
    QFile file( fileInfo.absoluteFilePath() );
    QVERIFY( file.open( QFile::ReadOnly ) );
    RedBullPlayer::Tools::RSS::RssFeed rssFeed( file.readAll() );
    file.close();
    QVERIFY( rssFeed.itemCount() == 3 );
    RedBullPlayer::Tools::RSS::IRssFeedItem* firstItem = rssFeed.itemAt( 0 );
    QVERIFY( firstItem->title() == "My First PodCast Item" );
    QVERIFY( firstItem->link() == "http://localhost/podcast/FirstItem.mp3" );
    QVERIFY( firstItem->guid() == "Unique Name for my first Item" );
    QVERIFY( firstItem->description() == "Description of my First PodCast Item" );
    QVERIFY( firstItem->enclosure() == "http://localhost/podcast/FirstItem.mp3" );
    QVERIFY( firstItem->enclosureContentType() == "audio/mpeg" );
    QVERIFY( firstItem->enclosureContentLength() == "450104" );
    QVERIFY( firstItem->category() == "Category of My First PodCast Item" );
    QVERIFY( firstItem->pubDate() == RedBullPlayer::Tools::RSS::RssFeed::dateTimeFromRFC822( "Mon, 22 Feb 2010 10:30:00 +0100" ) );
    RedBullPlayer::Tools::RSS::IRssFeedItem* secondItem = rssFeed.itemAt( 1 );
    QVERIFY( secondItem->title() == "My Second PodCast Item" );
    QVERIFY( secondItem->link() == "http://localhost/podcast/SecondItem.ogg" );
    QVERIFY( secondItem->guid() == "Unique Name for my second Item" );
    QVERIFY( secondItem->description() == "Description of my Second PodCast Item" );
    QVERIFY( secondItem->enclosure() == "http://localhost/podcast/SecondItem.ogg" );
    QVERIFY( secondItem->enclosureContentType() == "application/ogg" );
    QVERIFY( secondItem->enclosureContentLength() == "421571" );
    QVERIFY( secondItem->category() == "Category of My Second PodCast Item" );
    QVERIFY( secondItem->pubDate() == RedBullPlayer::Tools::RSS::RssFeed::dateTimeFromRFC822( "Mon, 22 Feb 2010 10:30:00 +0100" ) );
    /* missing
       - author
       - comments
       - source
       */
}
