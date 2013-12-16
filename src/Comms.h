/* 
 * File:   Comms.h
 * Author: aubrey
 *
 * Created on December 16, 2013, 5:35 PM
 */

#ifndef COMMS_H
#define	COMMS_H

namespace esp
{
	class EventStreamConsumer
	{
	public:
		/**
		 * Write a string into the database.
		 */
		virtual bool WriteString(uint32_t stringID, const char* string) = 0;
				
		/**
		 * Write an event into the database.
		 * 
         * @param event
         * @return 
         */
		virtual bool WriteEvent(const ProfileEvent& event) = 0;
	};
}

#endif	/* COMMS_H */

