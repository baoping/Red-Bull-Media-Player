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
#include "ArgumentParser.h"

// Qt includes
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QDebug>

// Project includes
#include "../Interfaces/IError.h"
#include "../Container/Constants.h"

using namespace RedBullPlayer::SnapshotCreator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer;
ArgumentParser::ArgumentParser( QObject *parent ) : QObject( parent ) {
}


SnapshotCreationData* ArgumentParser::parseCommand( QString command, RedBullPlayer::IError* error ) {
    //   qDebug() << command;
    error->reset();
    QDomDocument doc;
    bool worked = doc.setContent( command );

    if ( ! worked ) {
        error->setErrorMessage( tr( "Command is not a valid xml document" ) );
        return 0;
    }

    QDomElement commandElem = doc.firstChildElement();

    if ( commandElem.nodeName() != "command" ) {
        qWarning() << "commandElem.nodeName() != 'command'" ;
        error->setErrorMessage( tr( "Command xml does not start with the <command> tag" ) );
        return 0;
    }

    if ( ! commandElem.hasAttribute( "type" ) ) {
        qWarning() << "! commandElem.hasAttribute('type')" ;
        error->setErrorMessage( tr( "No type attribute found in command node" ) );
        return 0;
    }

    if ( ! commandElem.hasAttribute( "requestId" ) ) {
        qWarning() << "! commandElem.hasAttribute('requestId')" ;
        error->setErrorMessage( tr( "No requestId attribute found in command node" ) );
        return 0;
    }

    QString commandName = commandElem.attribute( "type" );
    qDebug() << "attrib requestId: " <<  commandElem.attribute( "requestId" );
    qDebug() << "attrib type:: " << commandElem.attribute( "type" );
    QUuid requestId( commandElem.attribute( "requestId" ) );
    qDebug() << "Uuid: " << requestId;

    if ( commandName != "snapshot" ) {
        qWarning() << "commandName != snapshot" ;
        error->setErrorMessage( tr( "Invalid command type" ) );
        return 0;
    }

    if ( requestId.isNull() ) {
        qDebug() << "requestId.isNull()";
        error->setErrorMessage( tr( "requestId does not conatin a valid uuid" ) );
        return 0;
    }

    QString movieFile;
    float position;
    QDomElement curr = commandElem.firstChildElement();

    while( ! curr.isNull() ) {
        //   qDebug() << "At elem: " << curr.nodeName();
        if ( curr.nodeName() == "moviefile" ) {
            QDomNode cdRaw = curr.firstChild();
            QDomCDATASection cd = cdRaw.toCDATASection();
            movieFile = cd.data();
            //          qDebug() << "Moviefilename parsed: " << movieFile;
        } else if( curr.nodeName() == "position" ) {
            QString strPos = curr.text();
            bool ok;
            position = strPos.toFloat( &ok );

            if ( ! ok ) {
                qWarning() << "Invalid postion string: " << strPos;
                error->setErrorMessage( tr( "position doies not contain a float number" ) );
                return 0;
            }

            //        qDebug() << "position parsed: " << position;
        }

        curr = curr.nextSiblingElement();
    }

    if ( movieFile.isEmpty() ) {
        qWarning() << "movieFile.isEmpty()";
        error->setErrorMessage( tr( "No moviefile element found in command" ) );
        return 0;
    }

    return new SnapshotCreationData( requestId, movieFile, position );
}
