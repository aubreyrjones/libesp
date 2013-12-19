#include "FileStore.h"
#include <stdlib.h>
#include <stdio.h>
#include "Event.h"
#include "sqlite3/sqlite3.h"
using namespace esp;

static const char* CREATE_EVENT_TABLE_QUERY = 
	"CREATE TABLE session_events ( "
	"id INTEGER PRIMARY KEY, "
	"eventType INTEGER, "
	"threadID INTEGER, "
	"parentEventRef INTEGER, "
	"frameNumber INTEGER, "
	"timestamp NUMERIC, "
	"eventNameRef INTEGER, "
	"value NUMERIC);";

static const char* CREATE_STRING_TABLE_QUERY = 
	"CREATE TABLE session_strings ( "
	"id INTEGER PRIMARY KEY, "
	"value TEXT );";

static const char* INSERT_EVENT_QUERY = 
	"INSERT INTO session_events VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

static const char* INSERT_STRING_QUERY = 
	"INSERT INTO session_strings VALUES (?, ?);";

SessionFileStore::SessionFileStore(const char* databaseFilename) : db(nullptr)
{
	int nameBufferLength = strlen(databaseFilename) + 1;
	filename = (char*) malloc(sizeof(char) * nameBufferLength);
	strncpy(filename, databaseFilename, nameBufferLength);
}

SessionFileStore::~SessionFileStore()
{
	free(filename);
	
	sqlite3_finalize(stringInsertStatement);
	sqlite3_finalize(eventInsertStatement);
	
	if (db){
		sqlite3_close(db);
	}
}

bool SessionFileStore::Initialize()
{
	int status;
	status = sqlite3_open(filename, &db);
	if (status != SQLITE_OK){
		printf("Error opening database: %s\n", sqlite3_errmsg(db));
		return false;
	}
	
	status = sqlite3_exec(db, CREATE_EVENT_TABLE_QUERY, nullptr, nullptr, nullptr);
	if (status != SQLITE_OK){
		printf("Error creating event table: %s\n", sqlite3_errmsg(db));
	}
	
	status = sqlite3_exec(db, CREATE_STRING_TABLE_QUERY, nullptr, nullptr, nullptr);
	if (status != SQLITE_OK){
		printf("Error creating string table: %s\n", sqlite3_errmsg(db));
	}
	
	status = sqlite3_prepare_v2(db, INSERT_EVENT_QUERY, -1, &eventInsertStatement, nullptr);
	if (status != SQLITE_OK){
		printf("Error creating event insert statement: %s\n", sqlite3_errmsg(db));
	}
	
	status = sqlite3_prepare_v2(db, INSERT_STRING_QUERY, -1, &stringInsertStatement, nullptr);
	if (status != SQLITE_OK){
		printf("Error creating string insert statement: %s\n", sqlite3_errmsg(db));
	}	
	
	return true;
}

bool SessionFileStore::WriteString(uint32_t stringID, const char* string)
{
	sqlite3_reset(stringInsertStatement);
	sqlite3_bind_int64(stringInsertStatement, 1, stringID);
	sqlite3_bind_text(stringInsertStatement, 2, string, -1, SQLITE_STATIC);
	
	try_step:
	int status = sqlite3_step(stringInsertStatement);
	if (status == SQLITE_DONE){
		return true;
	}
	else if (status == SQLITE_ERROR){
		printf("Error inserting string: %s\n", sqlite3_errmsg(db));
		return false;
	}
	else if (status == SQLITE_BUSY){
		goto try_step;
	}
	
	return false;
}

bool SessionFileStore::WriteEvent(const ProfileEvent& event)
{
	sqlite3_reset(eventInsertStatement);
	
	sqlite3_bind_int64(eventInsertStatement, 1, event.header.id);
	sqlite3_bind_int64(eventInsertStatement, 2, event.header.eventType);
	sqlite3_bind_int64(eventInsertStatement, 3, event.data.threadID);
	sqlite3_bind_int64(eventInsertStatement, 4, event.data.parentEventRef);
	sqlite3_bind_int64(eventInsertStatement, 5, event.data.frameNumber);
	sqlite3_bind_int64(eventInsertStatement, 6, event.data.timestamp);
	sqlite3_bind_int64(eventInsertStatement, 7, event.data.eventNameRef);
	sqlite3_bind_int64(eventInsertStatement, 8, event.data.value.ui);
	
	try_step:
	int status = sqlite3_step(eventInsertStatement);
	if (status == SQLITE_DONE){
		return true;
	}
	else if (status == SQLITE_ERROR){
		printf("Error inserting event: %s\n", sqlite3_errmsg(db));
		return false;
	}
	else if (status == SQLITE_BUSY){
		goto try_step;
	}
	
	return false;
}

		
void SessionFileStore::StartBulkInsert()
{
	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
}

void SessionFileStore::EndBulkInsert()
{
	sqlite3_exec(db, "COMMIT TRANSACTION;", nullptr, nullptr, nullptr);
}