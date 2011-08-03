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
#ifndef CONTENT_SQL_H
#define CONTENT_SQL_H

#include <QString>

#define CONTENT_TABLE_NAME (QString) "Content"

// Content SQL
#define SETUP_CONTENT_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS Content( " \
    "GUID TEXT PRIMARY KEY ASC," \
    "Name TEXT COLLATE NOCASE, " \
    "FileName TEXT COLLATE NOCASE, " \
    "DownloadUrl TEXT COLLATE NOCASE, " \
    "Media_GUID TEXT REFERENCES Media ( GUID ) ON DELETE CASCADE, " \
    "FileSize INTEGER, " \
    "Date INTEGER," \
    "State TEXT COLLATE NOCASE " \
    ");"

#define INSERT_CONTENT_TEMPLATE (QString) "INSERT INTO Content " \
    "( GUID, Name, FileName, DownloadUrl, Media_GUID, FileSize, Date, State ) VALUES" \
    "( :GUID, :Name, :FileName, :DownloadUrl, :Media_GUID, :FileSize, :Date, :State );"

#define INSERT_CONTENT_TEMPLATE_EXCLUDING_MEDIA_GUID (QString) "INSERT INTO Content " \
    "( GUID, Name, FileName, DownloadUrl, FileSize, Date, State ) VALUES" \
    "( :GUID, :Name, :FileName, :DownloadUrl, :FileSize, :Date, :State );"

#define SELECT_CONTENT_BY_GUID_TEMPLATE (QString) "SELECT * FROM Content " \
    "WHERE GUID=:GUID;"

#define SELECT_CONTENT_BY_STATE_TEMPLATE (QString) "SELECT * FROM Content " \
    "WHERE State=:STATE;"

#define UPDATE_CONTENT_TEMPLATE (QString) "UPDATE Content " \
    " SET FileName=:FileName, " \
    " Name=:Name, " \
    " DownloadUrl=:DownloadUrl, " \
    " Media_GUID=:Media_GUID, " \
    " FileSize=:FileSize, " \
    " Date=:Date, " \
    " State=:State " \
    " WHERE GUID=:GUID;"

#define UPDATE_CONTENT_TEMPLATE_EXCLUDING_MEDIA_GUID (QString) "UPDATE Content " \
    " SET FileName=:FileName, " \
    " Name=:Name, " \
    " DownloadUrl=:DownloadUrl, " \
    " FileSize=:FileSize, " \
    " Date=:Date, " \
    " State=:State " \
    " WHERE GUID=:GUID;"

#define DELETE_CONTENT_TEMPLATE (QString) "DELETE FROM Content WHERE GUID=:GUID;"

#endif // CONTENT_SQL_H
