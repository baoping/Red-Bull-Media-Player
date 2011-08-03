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
#include "ArgumentParserTest.h"


// Qt includes
#include <QDebug>
#include <QString>
#include <QDomDocument>

// Project includes

#include "../../Container/Error.h"
#include "SnapshotCreationData.h"
#include "ArgumentParser.h"

using namespace RedBullPlayer;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::SnapshotCreator;

void ArgumentParserTest::initTestCase() {
    requestId1 = "{11111111-1111-1111-1111-111111111111}";
    requestId2 = "{22222222-2222-2222-2222-222222222222}" ;
    requestId3 = "{33333333-3333-3333-3333-333333333333}";
    validCommand1 = "<command type=\"snapshot\" requestId=\"" + requestId1.toString() + "\">"
                    "<moviefile><![CDATA[testmovie1]]></moviefile>"
                    "<position>0.1</position>"
                    "</command>";
    validCommand2 = "<command type=\"snapshot\" requestId=\"" + requestId2.toString() + "\">"
                    "<moviefile><![CDATA[testmovie2]]></moviefile>"
                    "<position>0.2</position>"
                    "</command>";
    validCommand3 = "<command type=\"snapshot\" requestId=\"" + requestId3.toString() + "\">"
                    "<moviefile><![CDATA[testmovie3]]></moviefile>"
                    "<position>0.3</position>"
                    "</command>";
    invalid_command_no_snapshot_command = "<command type=\"snapshotasd\" requestId=\"" + requestId3.toString() + "\">"
                                          "<moviefile><![CDATA[testmovie1]]></moviefile>"
                                          "<position>0.1</position>"
                                          "</command>";
    invalid_command_no_xml = "<command type=\"snapshotasd\" requestId=\"" + requestId3.toString() + "\">"
                             "<moviefile><![CDATA[testmovie1]]></moviefile>"
                             "<position>0.1</position>"
                             "</coand>";
    invalid_command_no_movie_name = "<command type=\"snapshot\" requestId=\"" + requestId3.toString() + "\">"
                                    "<position>0.3</position>"
                                    "</command>";
    invalid_command_no_position    = "<command type=\"snapshot\" requestId=\"" + requestId3.toString() + "\">"
                                     "<moviefile><![CDATA[testmovie2]]></moviefile>"
                                     "</command>";
    invalid_command_wrong_command =  "<command type=\"snapshotasd\" requestId=\"" + requestId3.toString() + "\">"
                                     "<moviefile><![CDATA[testmovie3]]></moviefile>"
                                     "<position>0.3</position>"
                                     "</command>";
}

void ArgumentParserTest::should_create_valid_creationdata() {
    ArgumentParser ap( this );
    Error err;
    SnapshotCreationData* data1 = ap.parseCommand( validCommand1, &err );
    QVERIFY2( data1, "Must return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::NoError, "Must not set an error" );
    QVERIFY2( data1->movieFileName() == "testmovie1", "Must return corretct moviefilename" );
    QVERIFY2( data1->position() == 0.1F, "Must return corretct position" );
    QVERIFY2( data1->requestId() == requestId1, "Must return corretct requestid" );
    SnapshotCreationData* data2 = ap.parseCommand( validCommand2, &err );
    QVERIFY2( data2, "Must return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::NoError, "Must not set an error" );
    QVERIFY2( data2->movieFileName() == "testmovie2", "Must return corretct moviefilename" );
    QVERIFY2( data2->requestId() == requestId2, "Must return corretct requestid" );
    QVERIFY2( data2->position() == 0.2F, "Must return corretct position" );
    SnapshotCreationData* data3 = ap.parseCommand( validCommand3, &err );
    QVERIFY2( data3, "Must return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::NoError, "Must not set an error" );
    QVERIFY2( data3->movieFileName() == "testmovie3", "Must return corretct moviefilename" );
    QVERIFY2( data3->requestId() == requestId3, "Must return corretct requestid" );
    QVERIFY2( data3->position() == 0.3F, "Must return corretct position" );
}


void ArgumentParserTest::should_set_invalid_xml_error() {
    ArgumentParser ap( this );
    Error err;
    SnapshotCreationData* data1 = ap.parseCommand( invalid_command_no_xml, &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "Command is not a valid xml document" ), "Must return correct error message" );
}


void ArgumentParserTest::should_throw_error_on_invalid_command_tag() {
    ArgumentParser ap( this );
    Error err;
    SnapshotCreationData* data1;
    data1 = ap.parseCommand( "<comasdmand />", &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "Command xml does not start with the <command> tag" ), "Must return correct error message" );
    data1 = ap.parseCommand( "<command />", &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "No type attribute found in command node" ), "Must return correct error message" );
    data1 = ap.parseCommand( "<command type=\"snapshot\" />", &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "No requestId attribute found in command node" ), "Must return correct error message" );
    data1 = ap.parseCommand( "<command type=\"snapshot\" requestId=\"asdasdga\" />", &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "requestId does not conatin a valid uuid" ), "Must return correct error message" );
    data1 = ap.parseCommand( "<command type=\"snapshotasd\" requestId=\"" + requestId3.toString() + "\" />", &err );
    QVERIFY2( ! data1, "Must not return valid snapshotcreationdata" );
    QVERIFY2( err.errorType() == IError::Normal, "Must return Normal error" );
    QVERIFY2( err.errorMessage() == tr( "Invalid command type" ), "Must return correct error message" );
}
