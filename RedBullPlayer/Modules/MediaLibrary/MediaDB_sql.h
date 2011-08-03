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
#ifndef MEDIADB_SQL_H
#define MEDIADB_SQL_H

#include <QString>

#define SYSTEMMETADATA_TABLE_NAME (QString)"SystemMetaData"
#define MEDIA_TABLE_NAME (QString)"Media"
#define MEDIA_TYPE_TABLE_NAME (QString)"MediaType"
#define TAG_TABLE_NAME (QString)"Tag"
#define MEDIA_TAG_TABLE_NAME (QString)"MediaTag"
#define SNAPSHOT_TABLE_NAME (QString)"Snapshot"
#define NEWS_TABLE_NAME (QString) "News"
#define NEWS_IMAGE_TABLE_NAME (QString) "NewsImage"
#define STATISTIC_CLIENT_CACHE_TABLE_NAME (QString) "StatisticClientCache"

#define MEDIA_TYPE_VIDEO (QString)"946ac74e-8efd-4efe-b1f3-dec6afed53e2"
#define MEDIA_TYPE_AUDIO (QString)"6070a2a9-e1ed-4009-962f-de7dfddeaeb8"
#define MEDIA_TYPE_IMAGE (QString)"06792a7b-4d14-4ecf-9d70-a27fdf1ccc23"
#define MEDIA_TYPE_TV (QString)"64a84904-e275-4338-8d58-6bad18db0367"
#define MEDIA_TYPE_DOCUMENT (QString) "4e921750-dabf-11df-937b-0800200c9a66"

#define DB_VERSION_METADATA_DATAKEY (QString)"DBVersion"
#define DB_VERSION_METADATA_DATAVALUE (QString)"1.0"

// Version SQL

#define SETUP_SYSTEMMETADATA_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS SystemMetaData( " \
    "DataKey TEXT COLLATE NOCASE PRIMARY KEY ASC, " \
    "DataValue TEXT COLLATE NOCASE " \
    ");"

#define INSERT_SYSTEMMETADATA_TEMPLATE (QString) "INSERT INTO SystemMetaData " \
    "( DataKey, DataValue ) VALUES " \
    "( :DataKey, :DataValue );"

#define UPDATE_SYSTEMMETADATA_TEMPLATE (QString) "UPDATE SystemMetaData " \
    "SET " \
    "DataValue = :DataValue " \
    "WHERE DataKey = :DataKey;"

#define DELETE_SYSTEMMETADATA_TEMPLATE (QString) "DELETE FROM SystemMetaData " \
    "WHERE DataKey = :DataKey;"

#define SELECT_SYSTEMMETADATA_BY_DATAKEY (QString) "SELECT * FROM SystemMetaData " \
    "WHERE DATAKEY = :DATAKEY;"

// Statistic Client Cache
#define SETUP_STATISTIC_CLIENT_CACHE (QString) "CREATE TABLE IF NOT EXISTS StatisticClientCache ( " \
    "GUID TEXT PRIMARY KEY ASC," \
    "EntryType TEXT COLLATE NOCASE, " \
    "Data TEXT COLLATE NOCASE, " \
    "Date INTERGER NOT NULL " \
    ");"

#define INSERT_STATISTC_CLIENT_CACHE (QString) "INSERT INTO StatisticClientCache " \
    "( GUID, EntryType, Data, Date ) VALUES " \
    "( :GUID, :EntryType, :Data, :Date );"

#define DELETE_CLIENT_STATISTIC_CACHE (QString) "DELETE FROM StatisticClientCache " \
    "WHERE GUID=:GUID;"

#define SELECT_ALL_FROM_CLIENT_STATISTIC_CACHE (QString) "Select GUID, EntryType, Data, Date " \
    "FROM StatisticClientCache;"


// Media Type SQL
#define SETUP_MEDIATYPE_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS MediaType( " \
    "GUID TEXT PRIMARY KEY ASC," \
    "Name TEXT COLLATE NOCASE " \
    ");"

#define INSERT_MEDIA_TYPE_TEMPLATE (QString) "INSERT INTO MediaType " \
    "( GUID, Name ) VALUES" \
    "( :GUID, :Name );"

#define SELECT_MEDIA_TYPE_BY_GUID_TEMPLATE (QString) "SELECT GUID, Name FROM MediaType " \
    "WHERE GUID=:GUID;"

// Tag table SQL
#define SETUP_TAG_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS Tag( " \
    "GUID TEXT PRIMARY KEY ASC," \
    "Name TEXT COLLATE NOCASE, " \
    "BuiltIn INTEGER " \
    ");"

#define SELECT_TAG_BY_GUID_TEMPLATE (QString) "SELECT GUID, Name, BuiltIn FROM Tag " \
    "WHERE GUID=:GUID;"

#define INSERT_TAG_TEMPLATE (QString) "INSERT INTO Tag " \
    "( GUID, Name, BuiltIn ) VALUES" \
    "( :GUID, :Name, :BuiltIn );"

#define UPDATE_TAG_TEMPLATE (QString) "UPDATE OR FAIL Tag " \
    "SET Name=:Name, BuiltIn=:BuiltIn " \
    "WHERE GUID=:GUID"

#define DELETE_TAG_TEMPLATE (QString) "DELETE FROM Tag WHERE GUID=:GUID;"

#define SELECT_TAG_BY_MEDIA_TEMPLATE (QString) "SELECT GUID, Name, BuiltIn " \
    "FROM Tag INNER JOIN MediaTag " \
    "ON ( Tag.GUID = MediaTag.Tag_GUID ) " \
    "WHERE MediaTag.Media_GUID=:Media_GUID;"

// Media SQL
#define SETUP_MEDIA_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS Media(" \
    "GUID TEXT PRIMARY KEY ASC," \
    "FileName TEXT COLLATE NOCASE NOT NULL," \
    "MediaType TEXT COLLATE NOCASE NOT NULL REFERENCES MediaType ( GUID ) ON DELETE CASCADE," \
    "CoverImage BLOB," \
    "StartAnimation TEXT COLLATE NOCASE," \
    "Length INTEGER," \
    "Title TEXT COLLATE NOCASE," \
    "Artist TEXT COLLATE NOCASE," \
    "Genre TEXT COLLATE NOCASE," \
    "Copyright TEXT COLLATE NOCASE," \
    "Album TEXT COLLATE NOCASE," \
    "TrackNumber INTEGER," \
    "Description TEXT COLLATE NOCASE," \
    "Rating TEXT COLLATE NOCASE," \
    "Date INTEGER COLLATE NOCASE," \
    "Setting TEXT COLLATE NOCASE," \
    "Url TEXT COLLATE NOCASE," \
    "Language TEXT COLLATE NOCASE," \
    "Publisher TEXT COLLATE NOCASE," \
    "EncodedBy TEXT COLLATE NOCASE," \
    "ArtworkUrl TEXT COLLATE NOCASE," \
    "TrackID TEXT, " \
    "IsNew BOOL, " \
    "FileSize INTEGER, " \
    "StartTime INTEGER, " \
    "CreatedAt INTEGER, " \
    "AnimationFrameCount INTEGER " \
    ");"

#define OPTIMIZE_MEDIA_TABLE_SQL_1 (QString) "CREATE INDEX IF NOT EXISTS idxUiSearch ON Media(IsNew, CreatedAt);"

#define SELECT_ISONLINECONTENT_SQL (QString) "SELECT count(*) as IsOnlineContent from CONTENT where MEDIA_GUID = :MEDIA_GUID;"

//DO NOT FORGET TO UPDATE INSERT_MEDIA_TEMPLATE, UPDATE_MEDIA_TEMPLATE and SELECT_MEDIA_BY_TAG_TEMPLATE
//IF YOU ADD A COLUMN TO MEDIA TABLE

#define INSERT_MEDIA_TEMPLATE (QString) "INSERT INTO Media " \
    "( GUID, FileName, MediaType, Length, Title, Artist, Genre, Copyright, Album, TrackNumber, Description, Rating, Date, Setting, Url, Language, Publisher, EncodedBy, ArtworkUrl, TrackID, IsNew, FileSize, StartTime, CreatedAt, StartAnimation, AnimationFrameCount ) VALUES" \
    "( :GUID, :FileName, :MediaType, :Length, :Title, :Artist, :Genre, :Copyright, :Album, :TrackNumber, :Description, :Rating, :Date, :Setting, :Url, :Language, :Publisher, :EncodedBy, :ArtworkUrl, :TrackID, :IsNew, :FileSize, :StartTime, :CreatedAt, :StartAnimation, :AnimationFrameCount);"

#define UPDATE_MEDIA_TEMPLATE (QString) "UPDATE Media " \
    " SET Length=:Length, " \
    " Title=:Title, " \
    " Artist=:Artist, " \
    " Genre=:Genre, " \
    " Copyright=:Copyright, " \
    " Album=:Album, " \
    " TrackNumber=:TrackNumber, " \
    " Description=:Description, " \
    " Date=:Date, " \
    " Setting=:Setting, " \
    " Url=:Url, " \
    " Language=:Language, " \
    " Publisher=:Publisher, " \
    " EncodedBy=:EncodedBy, " \
    " ArtworkUrl=:ArtworkUrl, " \
    " TrackID=:TrackID, " \
    " Rating=:Rating, " \
    " IsNew=:IsNew, " \
    " FileSize=:FileSize, " \
    " StartTime=:StartTime, " \
    " StartAnimation=:StartAnimation, " \
    " AnimationFrameCount=:AnimationFrameCount" \
    " WHERE GUID=:GUID;"

#define UPDATE_MEDIA_COVER_TEMPLATE  (QString) "UPDATE Media " \
    "SET CoverImage=:CoverImage " \
    " WHERE GUID=:GUID;"


#define SELECT_MEDIA_BY_TAG_TEMPLATE (QString) "SELECT " \
    " GUID, FileName, MediaType, Length, Artist, Genre, Copyright, Album, TrackNumber, Description, Rating, Date, Setting, Url, Language, Publisher, EncodedBy, ArtworkUrl, TrackID, CoverImage, StartAnimation, IsNew, FileSize, StartTime, CreatedAt " \
    " FROM Media INNER JOIN MediaTag " \
    " ON ( Media.GUID = MediaTag.Media_GUID ) " \
    " WHERE MediaTag.Tag_GUID=:Tag_GUID;"

#define SELECT_ALL_MEDIA_IN_UI_ORDER (QString) "SELECT GUID, FileName FROM Media Order By CreatedAt desc;"

#define SELECT_MEDIA_FOR_QUICK_DB_SEARCH (QString) "SELECT GUID FROM Media " \
    "WHERE Title LIKE :Query OR " \
    " Artist LIKE :Query " \
    "Order By IsNew desc, CreatedAt desc;"

#define DELETE_MEDIA_TEMPLATE (QString) "DELETE FROM Media WHERE GUID=:GUID;"

// Media Tag Table sql
#define SETUP_MEDIATAG_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS MediaTag( " \
    "Tag_GUID TEXT REFERENCES Tag ( GUID ) ON DELETE CASCADE," \
    "Media_GUID TEXT NOT NULL REFERENCES Media ( GUID ) ON DELETE CASCADE, " \
    "CONSTRAINT S_PK PRIMARY KEY( Tag_GUID, Media_GUID )" \
    ");"

#define INSERT_MEDIATAG_TEMPLET (QString) "INSERT INTO MediaTag " \
    "( Tag_GUID, Media_GUID ) VALUES" \
    "( :Tag_GUID, :Media_GUID );"

#define DELETE_MEDIATAG_TEMPLET (QString) "DELETE FROM MediaTag " \
    "WHERE Tag_GUID=:Tag_GUID AND Media_GUID=:Media_GUID;"

#define DELETE_MEDIATAG_BY_MEDIA_TEMPLATE (QString) "DELETE FROM MediaTag " \
    "WHERE Media_GUID=:Media_GUID;"

#define DELETE_MEDIATAG_BY_TAG_TEMPLATE (QString) "DELETE FROM MediaTag " \
    "WHERE Tag_GUID=:Tag_GUID;"


// Snapshot Table

#define SETUP_SNAPSHOT_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS Snapshot ( " \
    "GUID TEXT PRIMARY KEY ASC," \
    "Media_GUID TEXT NOT NULL REFERENCES Media ( GUID ) ON DELETE CASCADE, " \
    "Snapshot BLOB, " \
	"Position REAL " \
    ");"

#define INSERT_SNAPSHOT_TEMPLATE (QString) "INSERT INTO Snapshot " \
        "( GUID, Media_GUID, Snapshot, Position ) VALUES " \
	"( :Tag_GUID, :Media_GUID, :Snapshot, :Position );"

#define UPDATE_SNAPSHOT_TEMPLATE  (QString) "UPDATE Snapshot " \
    "SET Media_GUID=:Media_GUID, " \
    "Snapshot=:Snapshot, " \
	"Position=:Position " \
    "WHERE GUID=:GUID;"

#define DELETE_SNAPSHOT_TEMPLATE (QString) "DELETE FROM Snapshot " \
    "WHERE GUID=:GUID;"

// News table sqls

#define SETUP_NEWS_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS News ( " \
    "GUID TEXT PRIMARY KEY ASC, " \
    "Title TEXT COLLATE NOCASE NOT NULL, " \
    "Link TEXT COLLATE NOCASE, " \
    "Description TEXT COLLATE NOCASE, " \
    "Content TEXT COLLATE NOCASE, " \
    "Author TEXT COLLATE NOCASE, " \
    "Category TEXT COLLATE NOCASE, " \
    "External_GUID TEXT COLLATE NOCASE, " \
    "PubDate INTEGER " \
    ");"

#define INSERT_NEWS_TEMPLATE (QString) "INSERT INTO News " \
    "( GUID, Title, Link, Description, Content, Author, Category, External_GUID, PubDate ) VALUES " \
    "( :GUID, :Title, :Link, :Description, :Content, :Author, :Category, :External_GUID, :PubDate);"

#define UPDATE_NEWS_TEMPLATE (QString) "UPDATE News " \
    "SET " \
    "Title = :Title, " \
    "Link = :Link, " \
    "Description = :Description, " \
    "Content = :Content, " \
    "Author = :Author, " \
    "Category = :Category, " \
    "External_GUID = :External_GUID, " \
    "PubDate = :PubDate " \
    "WHERE GUID = :GUID;"

#define DELETE_NEWS_TEMPLATE (QString) "DELETE FROM News WHERE GUID=:GUID;"

#define SELECT_NEWS_BY_GUID_SQL (QString) "SELECT * FROM News WHERE GUID=:GUID;"

#define SELECT_NEWS_COUNT_SQL (QString) "SELECT count(GUID) as c FROM NEWS WHERE GUID=:GUID;"

#define SELECT_ALL_NEWS_SQL (QString) "SELECT * FROM News;"

#define SELECT_ALL_NEWS_ORDER_BY_PUBDATE_SQL (QString) "SELECT * FROM News Order By PubDate desc;"

// NewsImage table sqls

#define SETUP_NEWS_IMAGE_TABLE_SQL (QString) "CREATE TABLE IF NOT EXISTS NewsImage ( " \
    "GUID TEXT PRIMARY KEY ASC, " \
    "News_GUID TEXT NOT NULL REFERENCES News ( GUID ) ON DELETE CASCADE, " \
    "FileName TEXT COLLATE NOCASE NOT NULL, " \
    "Width INTEGER, " \
    "Height INTEGER, " \
    "Size INTEGER, " \
    "Image BLOB " \
    ");"

#define INSERT_NEWS_IMAGE_TEMPLATE (QString) "INSERT INTO NewsImage " \
    "( GUID, News_GUID, FileName, Width, Height, Size ) VALUES " \
    "( :GUID, :News_GUID, :FileName, :Width, :Height, :Size);"

#define UPDATE_NEWS_IMAGE_TEMPLATE (QString) "UPDATE NewsImage " \
    "SET " \
    "News_GUID = :News_GUID, " \
    "FileName = :FileName, " \
    "Width = :Width, " \
    "Height = :Height, " \
    "Size = :Size " \
    "WHERE GUID = :GUID;"

#define UPDATE_NEWS_IMAGE_IMAGE_TEMPLATE (QString) "UPDATE NewsImage " \
    "SET Image = :Image where GUID = :GUID;"

#define DELETE_NEWS_IMAGE_TEMPLATE (QString) "DELETE FROM NewsImage WHERE GUID=:GUID;"

#define DELETE_NEWS_IMAGE_BY_NEWS_TEMPLATE (QString) "DELETE FROM NewsImage WHERE News_GUID=:News_GUID;"

#define SELECT_NEWS_IMAGE_BY_GUID_SQL (QString) "SELECT * FROM NewsImage WHERE GUID = :GUID;"

#define SELECT_NEWS_IMAGES_BY_NEWS_GUID_SQL (QString) "SELECT * FROM NewsImage WHERE News_GUID = :News_GUID;"

#define SELECT_NEWS_IMAGES_COUNT_BY_NEWS_GUID_SQL (QString) "SELECT count(GUID) as c FROM NewsImage WHERE News_GUID = :News_GUID;"

#define SELECT_NEWS_IMAGE_COUNT_SQL (QString) "SELECT count(GUID) as c FROM NewsImage WHERE GUID=:GUID;"

#define CLEAR_NEWS_IMAGE_CACHE (QString) "DELETE FROM NewsImage;"

#define CLEAR_NEWS_CACHE (QString) "DELETE FROM News;"



#endif // MEDIADB_SQL_H
