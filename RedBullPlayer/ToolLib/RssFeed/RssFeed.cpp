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
#include "RssFeed.h"

#include <QDebug>
#include <QLocale>

#include "RssFeedImageDescriptor.h"
#include "RssFeedItem.h"

#include "../../Container/Constants.h"
#include "../SimpleLogger.h"

using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

RssFeed::RssFeed( QByteArray rawFeedData ) {
    _imageDescriptor = NULL;
    _items = new QList<IRssFeedItem*>();
    parseRawData( rawFeedData );
}

RssFeed::~RssFeed() {
    if ( _imageDescriptor != NULL )
        delete _imageDescriptor;

    while ( !_items->empty() ) {
        delete _items->takeFirst();
    }

    delete _items;
}

QDateTime RssFeed::dateTimeFromRFC822( QString dateTimeStr ) {
    QString cut = dateTimeStr.remove( dateTimeStr.lastIndexOf( " " ), 6 );
    QString dayPreset = dateTimeStr.left( 5 );
    cut = cut.remove( dayPreset ).trimmed();
    QLocale us = QLocale( QLocale::English, QLocale::UnitedStates );
    QDateTime result = us.toDateTime( cut, "dd MMM yyyy HH:mm:ss" );
    return result;
}

IRssFeed* RssFeed::parseFeed( QByteArray rawFeedData ) {
    return new RssFeed( rawFeedData );
}

void RssFeed::parseRawData( QByteArray rawData ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [START] RawData ***************" );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, rawData );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [START] RawData ***************" );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [START] RSS Feed ***************" );
    QXmlStreamReader xml( rawData );

    while ( !xml.atEnd() ) {
        xml.readNext(); //read next token

        if ( xml.isStartElement() && ( xml.prefix() != "itunes" ) ) {
            QString xmlName = xml.name().toString();

            if ( xmlName == "title" ) {
                _title = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _title ) );
            } else if ( xmlName == "link" ) {
                _link = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _link ) );
            } else if ( xmlName == "description" ) {
                _description = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _description ) );
            } else if ( xmlName == "language" ) {
                _language = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _language ) );
            } else if ( xmlName == "copyright" ) {
                _copyright = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _copyright ) );
            } else if ( xmlName == "managingEditor" ) {
                _managingEditor = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _managingEditor ) );
            } else if ( xmlName == "webMaster" ) {
                _webMaster = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _webMaster ) );
            } else if ( xmlName == "pubDate" ) {
                _pubDateString = xml.readElementText();
                _pubDate = dateTimeFromRFC822( _pubDateString );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2 | %3" ).arg( xmlName ).arg( _pubDateString ).arg( _pubDate.toString() ) );
            } else if ( xmlName == "category" ) {
                _category = ""; //TODO implement error handling for wrong category
            } else if ( xmlName == "image" ) {
                parseImageElement( &xml );
            } else if ( xmlName == "rating" ) {
                _rating = xml.readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( _rating ) );
            } else if ( xmlName == "item" ) {
                parseItemElement( &xml );
            }
        }
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [END] RSS Feed ***************" );
}

void RssFeed::parseImageElement( QXmlStreamReader* xml ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [START] Image Element ***************" );
    QString url = "";
    QString title = "";
    QString link = "";
    int width = 0;
    int height = 0;

    while ( !( xml->isEndElement() && xml->name() == "image" ) ) {
        xml->readNext();

        if ( xml->isStartElement() && ( xml->prefix() != "itunes" ) ) {
            QString xmlName = xml->name().toString();

            if ( xmlName == "url" ) {
                url = xml->readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( url ) );
            } else if ( xmlName == "title" ) {
                title = xml->readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( title ) );
            } else if ( xmlName == "link" ) {
                link = xml->readElementText();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( link ) );
            } else if ( xmlName == "width" ) {
                width = xml->readElementText().toInt();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( width ) );
            } else if ( xmlName == "height" ) {
                height = xml->readElementText().toInt();
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( height ) );
            }
        }
    }

    _imageDescriptor = new RssFeedImageDescriptor( url, title, link, width, height );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [END] Image Element ***************" );
}

void RssFeed::parseItemElement( QXmlStreamReader* xml ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [START] Item Element ***************" );
    RssFeedItem* item = new RssFeedItem( );

    while ( !( xml->isEndElement() && xml->name() == "item" ) ) {
        xml->readNext();

        if ( xml->isStartElement() && ( xml->prefix() != "itunes" ) ) {
            QString xmlName = xml->name().toString();

            if ( xmlName == "title" ) {
                item->setTitle( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->title() ) );
            } else if ( xmlName == "guid" ) {
                item->setGuid( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->guid() ) );
            } else if ( xmlName == "link" ) {
                item->setLink( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->link() ) );
            } else if ( xmlName == "enclosure" ) {
                item->setEnclosure( xml->attributes().value( "url" ).toString() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( "enclosure.url" ).arg( item->enclosure() ) );
                item->setEnclosureContentType( xml->attributes().value( "type" ).toString() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( "enclosure.type" ).arg( item->enclosureContentType() ) );
                item->setEnclosureContentLength( xml->attributes().value( "length" ).toString() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( "enclosure.length" ).arg( item->enclosureContentLength() ) );
            } else if ( xmlName == "description" ) {
                item->setDescription( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->description() ) );
            } else if ( ( xml->prefix() == "content" ) && ( xmlName == "encoded" ) ) {
                item->setContentEncoded( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->contentEncoded() ) );
            } else if ( xmlName == "author" ) {
                item->setAuthor( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->author() ) );
            } else if ( xmlName == "pubDate" ) {
                item->setPubDateString( xml->readElementText() );
                item->setPubDate( dateTimeFromRFC822( item->pubDateString() ) );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->pubDateString() ) );
            } else if ( ( xml->prefix() == "redbull" ) && ( xmlName == "image" ) ) {
                item->setRedBullImageUrl( xml->attributes().value( "href" ).toString() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( xml->attributes().value( "href" ).toString() ) );
            } else if ( xmlName == "category" ) {
                item->setCategory( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->category() ) );
            } else if ( xmlName == "source" ) {
                item->setSource( xml->readElementText() );
                SimpleLogger::instance()->debug( Constants::LOGGER_RSS, QString( "Found %1 Tag. Value: %2" ).arg( xmlName ).arg( item->source() ) );
            }
        }
    }

    _items->append( item );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "*************** [END] Item Element ***************" );
}
