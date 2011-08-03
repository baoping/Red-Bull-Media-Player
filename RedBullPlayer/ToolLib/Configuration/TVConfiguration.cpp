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
#include "TVConfiguration.h"

#include <QFileInfo>

#include "../helper.h"

using namespace RedBullPlayer::Tools::Configuration::TV;

TVConfiguration::TVConfiguration( QSettings* settings, QObject *parent ) : QObject( parent ) {
    _channels = new QList<TVChannel*>();
    TVChannel* c1 = new TVChannel();
    c1->setName( settings->value( "Playlist/channel1Name", "" ).toString() );
    c1->setUrl( Helper::decodeUrl( settings->value( "Playlist/channel1Url", "" ).toString().toUtf8() ) );
    c1->setImage( new QImage( settings->value( "Playlist/channel1Thumbnail", "" ).toString() ) );
    c1->setStartAnimationFile( settings->value( "Playlist/channel1StartAnimation", "" ).toString() );
    c1->setAnimationFrameCount( settings->value( "Playlist/channel1AnimationFrameCount", "" ).toInt() );
    _channels->append( c1 );

    /*TVChannel* c2 = new TVChannel();
    c2->setName( settings->value("Playlist/channel2Name","").toString() );
    c2->setUrl( Helper::decodeUrl(settings->value("Playlist/channel2Url","").toString().toUtf8()) );
    c2->setImage( new QImage(settings->value("Playlist/channel2Thumbnail","").toString()) );
    c2->setStartAnimationFile( settings->value("Playlist/channel2StartAnimation","").toString() );
    c2->setAnimationFrameCount( settings->value("Playlist/channel2AnimationFrameCount","").toInt() );
    _channels->append( c2 );*/
}

TVConfiguration::~TVConfiguration() {
    while ( !_channels->isEmpty() )
        _channels->takeFirst()->deleteLater();

    delete _channels;
}

int TVConfiguration::count() {
    return _channels->count();
}

QString TVConfiguration::nameAt( int index ) {
    return ( validIndex( index ) ) ? _channels->at( index )->name() : "";
}

QUrl TVConfiguration::urlAt( int index ) {
    return ( validIndex( index ) ) ? _channels->at( index )->url() : QUrl( "" );
}

QImage TVConfiguration::imageAt( int index ) {
    return ( validIndex( index ) ) ? *_channels->at( index )->image() : QImage();
}

QString TVConfiguration::startAnimationFileAt( int index ) {
    return ( validIndex( index ) ) ? _channels->at( index )->startAnimationFile() : "";
}

int TVConfiguration::animationFrameCountAt( int index ) {
    return ( validIndex( index ) ) ? _channels->at( index )->animationFrameCount() : 0;
}

bool TVConfiguration::validIndex( int index ) {
    return ( ( index >= 0 ) && ( index < _channels->count() ) );
}
