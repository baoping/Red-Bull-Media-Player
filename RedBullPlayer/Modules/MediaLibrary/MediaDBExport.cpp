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
#include "MediaDBExport.h"

#include <QSqlDatabase>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QFile>

#include "MediaDB_sql.h"

using namespace RedBullPlayer::Modules::MediaLibrary;

MediaDBExport::MediaDBExport( QObject *parent ) :
    QObject( parent ) {
}

bool MediaDBExport::exportToXml( QString fileName ) {
    QSqlDatabase db = QSqlDatabase::database();
    QByteArray document;
    QXmlStreamWriter docStream( &document );
    docStream.setAutoFormatting( true );
    docStream.writeStartDocument();
    QSqlTableModel systemMetaData;
    systemMetaData.setTable( "SystemMetaData" );
    systemMetaData.setFilter( QString( "DataKey='%1'" ).arg( DB_VERSION_METADATA_DATAKEY ) );
    systemMetaData.select();
    docStream.writeTextElement( "DBVersion", systemMetaData.record( 0 ).value( "DataValue" ).toString() );
    QStringList tables = db.tables();

    for( int i = 0; i < tables.count(); i++ ) {
        QString tableName = tables.at( i );

        if ( tableName.compare( "SystemMetaData", Qt::CaseInsensitive ) != 0 ) {
            QSqlTableModel tableModel;
            tableModel.setTable( tableName );
            tableModel.select();
            tableDataToXml( &docStream, &tableModel );
        }
    }

    docStream.writeEndDocument();

    if ( QFile::exists( fileName ) )
        QFile::remove( fileName );

    QFile file( fileName );
    bool success = file.open( QFile::WriteOnly );

    if ( success ) {
        file.write( document );
        file.close();
    }

    return success;
}

void MediaDBExport::tableDataToXml( QXmlStreamWriter* docStream, QSqlTableModel* tableModel ) {
    docStream->writeStartElement( "Table" );
    docStream->writeAttribute( "name", tableModel->tableName() );

    for( int i = 0; i < tableModel->rowCount(); i++ ) {
        QSqlRecord record = tableModel->record( i );
        recordDataToXml( docStream, &record );
    }

    docStream->writeEndElement(); //Table
}

void MediaDBExport::recordDataToXml( QXmlStreamWriter* docStream, QSqlRecord* record ) {
    docStream->writeStartElement( "Row" );

    for( int i = 0; i < record->count(); i++ ) {
        QSqlField field = record->field( i );
        fieldDataToXml( docStream, &field );
    }

    docStream->writeEndElement(); //Row;
}

void MediaDBExport::fieldDataToXml( QXmlStreamWriter* docStream, QSqlField* field ) {
    docStream->writeStartElement( "Field" );
    docStream->writeAttribute( "name", field->name() );
    docStream->writeStartElement( "Value" );
    docStream->writeAttribute( "variantType", QVariant::typeToName( field->type() ) );

    if ( !field->isNull() ) {
        if ( field->type() == QVariant::ByteArray ) {
            QByteArray array = field->value().toByteArray().toBase64();
            docStream->writeCDATA( QString( array ) );
        } else {
            docStream->writeCDATA( field->value().toString().toLatin1() );
        }
    }

    docStream->writeEndElement(); //Value;
    docStream->writeEndElement(); //Field;
}
