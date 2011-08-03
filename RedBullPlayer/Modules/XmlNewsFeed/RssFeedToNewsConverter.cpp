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
#include "RssFeedToNewsConverter.h"

#include <QDomDocument>
#include <QImage>
#include <QFileInfo>

#include "../../ToolLib/helper.h"
#include "../../Interfaces/MediaLibrary/INewsImage.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::XmlNewsFeed;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Container;

RssFeedToNewsConverter::RssFeedToNewsConverter( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent ) :
    QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    _imageLoader = new DownloadUtil( this );
    connect( _imageLoader, SIGNAL( downloadFinished( QString, QString, QString ) ), this, SLOT( downloadFinished( QString, QString, QString ) ) );
    connect( _imageLoader, SIGNAL( downloadFailed( QString, int, QString ) ), this, SLOT( downloadFailed( QString, int, QString ) ) );
}


RssFeedToNewsConverter::~RssFeedToNewsConverter() {
    _imageLoader->deleteLater();
}


void RssFeedToNewsConverter::convert( IRssFeedItem* rssFeedItem ) {
    _currentItem = rssFeedItem;
    _imagesToLoad.clear();
    _imagesLoaded.clear();
    QDomDocument doc;
    QString content;

    if ( rssFeedItem->contentEncoded() != "" ) {
        content = rssFeedItem->contentEncoded();
    } else {
        content = rssFeedItem->description();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "News content: " + content );
    doc.setContent( content );
    QDomNodeList imageElements = doc.elementsByTagName( "img" );
    SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "ImageCount: " + imageElements.count() );

    for( uint i = 0; i < imageElements.length(); i++ ) {
        QDomElement e = imageElements.at( i ).toElement();
        QString source = e.attribute( "src" );
        SimpleLogger::instance()->debug( Constants::LOGGER_RSS, "Found Image: " + source );

        if ( !source.isEmpty() ) {
            _imagesToLoad.enqueue( source );
        }
    }

    if ( _imagesToLoad.count() > 0 ) {
        loadNextImage();
    } else {
        convertToNews();
    }
}

void RssFeedToNewsConverter::loadNextImage() {
    if ( _imagesToLoad.count() == 0 ) {
        convertToNews();
    } else {
        _imageLoader->downloadFile( Helper::decodeUrl( _imagesToLoad.dequeue() ) );
    }
}

void RssFeedToNewsConverter::convertToNews() {
    INews* news = _mediaLibrary->mediaDB()->newsRepository()->createNews( RedBullPlayer::Tools::UuidCreator::create(), _currentItem->title() );
    news->setAuthor( _currentItem->author() );
    news->setCategory( _currentItem->category() );

    if ( _currentItem->contentEncoded() != "" ) {
        news->setDescription( _currentItem->description() );
        news->setContent( _currentItem->contentEncoded() );
    } else {
        news->setDescription( "" );
        news->setContent( _currentItem->description() );
    }

    news->setExternalGuid( _currentItem->guid() );
    news->setLink( _currentItem->link() );
    news->setPubDate( _currentItem->pubDate() );
    QList<INewsImage*>* newsImages = new QList<INewsImage*>();

    for( int i = 0; i < _imagesLoaded.count(); i++ ) {
        INewsImage* newsImage = _mediaLibrary->mediaDB()->newsRepository()->createNewsImage( news->guid(), RedBullPlayer::Tools::UuidCreator::create(), QFileInfo( _imagesLoaded.at( i ) ).fileName() );
        QImage img( _imagesLoaded.at( i ) );
        newsImage->setImage( img );
        newsImage->setWidth( img.width() );
        newsImage->setHeight( img.height() );
        newsImage->setSize( QFileInfo( newsImage->fileName() ).size() );
        newsImages->append( newsImage );
    }

    emit convertFinished( news, newsImages );
}

void RssFeedToNewsConverter::downloadFinished( QString fileName, QString contentType, QString contentLength ) {
    _imagesLoaded.append( fileName );
    loadNextImage();
}

void RssFeedToNewsConverter::downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase ) {
    loadNextImage();
}
