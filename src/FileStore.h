/* 
 * File:   FileStore.h
 * Author: aubrey
 *
 * Created on December 16, 2013, 3:21 PM
 */

#ifndef FILESTORE_H
#define	FILESTORE_H

#include "sqlite3/sqlite3.h"
#include "Event.h"
#include <string.h>
#include "Comms.h"

namespace esp
{
	class SessionFileStore : public EventStreamConsumer
	{
	private:
		sqlite3 *db;
		sqlite3_stmt *eventInsertStatement, *stringInsertStatement;
		char *filename;
		
	public:
		
		SessionFileStore(const char* databaseFilename);
		virtual ~SessionFileStore();
		
		/**
		 * Open the database file.
         * @return 
         */
		bool Initialize();
		
		/**
		 * Get the filename used to open the database.
         * @return 
         */
		const char* GetDatabaseFilename() const
		{
			return filename;
		}
				
		/**
		 * Write a string into the database.
		 */
		bool WriteString(uint32_t stringID, const char* string);
				
		/**
		 * Write an event into the database.
		 * 
         * @param event
         * @return 
         */
		bool WriteEvent(const ProfileEvent& event);
	};
}

#endif	/* FILESTORE_H */

