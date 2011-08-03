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
#include "ArchivIndex.h"


// QT includes
#include <QDebug>
#include <QTextStream>
#include <QDomElement>

const QString ArchivIndex::DOCUMENT_ELEMENT = "index";
const QString ArchivIndex::FILE_REFERENCE_ELEMENT = "file";
const QString ArchivIndex::FILENAME_ATTRIBUTE = "fileName";
const QString ArchivIndex::EXTRACTFILENAME_ATTRIBUTE = "extractFileName";
const QString ArchivIndex::OFFSET_ATTRIBUTE = "offset";
const QString ArchivIndex::SIZE_ATTRIBUTE = "size";
const QString ArchivIndex::EXTRACTEDSIZE_ATTRIBUTE = "extractedSize";
ArchivIndex::ArchivIndex() {
    _index.appendChild( _index.createElement( DOCUMENT_ELEMENT ) );
}

ArchivIndex::ArchivIndex( QByteArray indexData ) {
    if ( ! _index.setContent( indexData ) ) {
        qFatal( "Could not parse Index XML" + indexData );
    }
}

void ArchivIndex::addFileReference( ArchivFileReference* archivFile ) {
    Q_ASSERT( archivFile );
    QDomElement elem = _index.createElement( FILE_REFERENCE_ELEMENT );
    archivFile->setupXmlElement( &elem );
    _index.documentElement().appendChild( elem );
}

void ArchivIndex::removeFileReference( QString fileName ) {
    //    qDebug() << "Try Remove file: " << fileName;
    QDomElement currElem = _index.documentElement().firstChildElement();
    ArchivFileReference* toRemove = 0;

    while( ! currElem.isNull() && toRemove == 0 ) {
        if ( currElem.nodeName() == ArchivIndex::FILE_REFERENCE_ELEMENT ) {
            toRemove = ArchivFileReference::loadFromXmlElement( currElem );

            if ( toRemove->fileName() == fileName ) {
                break;
            }

            delete toRemove;
            toRemove = 0;
        }

        currElem = currElem.nextSiblingElement();
    }

    if ( ! toRemove || currElem.isNull() ) {
        qWarning() << "No file found for Filename: " << fileName;
        return;
    }

    QDomElement toRemoveElement = currElem;

    while( ! currElem.isNull() ) {
        if ( currElem.nodeName() == ArchivIndex::FILE_REFERENCE_ELEMENT ) {
            ArchivFileReference* curr = ArchivFileReference::loadFromXmlElement( currElem );
            currElem.setAttribute( ArchivIndex::OFFSET_ATTRIBUTE, curr->offset() - toRemove->fileSize() );
            delete curr;
        }

        currElem = currElem.nextSiblingElement();
    }

    _index.documentElement().removeChild( toRemoveElement );
}

void ArchivIndex::saveIndex( QFile* targetFile ) {
    if ( ! targetFile->open( QIODevice::WriteOnly ) ) {
        qFatal( "Could not save index file" );
    }

    QTextStream ts( targetFile );
    ts << _index.toString();
    targetFile->close();
    //  qDebug() << "Wrote index xml: " << _index.toString();
}

QByteArray ArchivIndex::compressedIndexData() {
    return qCompress( _index.toByteArray( -1 ), ArchivFile::COMPRESSION_RATE );
}

QList<ArchivFileReference*>* ArchivIndex::fileReferences() const {
    QList<ArchivFileReference*>* ret = new QList<ArchivFileReference*>();
    QDomElement currElem = _index.documentElement().firstChildElement();

    while( ! currElem.isNull() ) {
        if ( currElem.nodeName() == ArchivIndex::FILE_REFERENCE_ELEMENT ) {
            ret->append( ArchivFileReference::loadFromXmlElement( currElem ) );
        }

        currElem = currElem.nextSiblingElement();
    }

    return ret;
}

ArchivFileReference* ArchivIndex::getFileReferenceByFileName( QString fileName ) const {
    //    qDebug() << "Index Lookup file: " << fileName;
    QDomElement currElem = _index.documentElement().firstChildElement();

    while( ! currElem.isNull() ) {
        if ( currElem.nodeName() == ArchivIndex::FILE_REFERENCE_ELEMENT ) {
            ArchivFileReference* ref = ArchivFileReference::loadFromXmlElement( currElem );

            //	    qDebug() << "   At file: " << ref->fileName();
            if ( ref->fileName() == fileName ) {
                return ref;
            } else {
                delete ref;
            }
        }

        currElem = currElem.nextSiblingElement();
    }

    return 0;
}

int ArchivIndex::fileCount() const {
    return _index.documentElement().childNodes().count();
}

int ArchivIndex::extracedContentSize() const {
    int size = 0;
    QDomElement currElem = _index.documentElement().firstChildElement();

    while( ! currElem.isNull() ) {
        if ( currElem.nodeName() == ArchivIndex::FILE_REFERENCE_ELEMENT ) {
            ArchivFileReference* ref = ArchivFileReference::loadFromXmlElement( currElem );
            size += ref->extractedFileSize();
            delete ref;
        }

        currElem = currElem.nextSiblingElement();
    }

    return size;
}
