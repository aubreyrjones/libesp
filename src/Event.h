/* 
 * File:   Event.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 1:38 PM
 */

#include <stdint.h>
#include "queue/readerwriterqueue.h"

#ifndef EVENT_H
#define	EVENT_H

namespace esp
{
	enum EventType : uint32_t
	{
		EV_PROBE_UINT = 1,
		EV_PROBE_FLOAT = 2,
		EV_PROBE_INT = 3,
		EV_ZONE_INTERVAL = 4
	};
	
	union ProfileEventValue
	{
		float f;
		uint32_t ui;
		int32_t i;
	};

	/**
	 * A profile event is a single sample coming from within the target application.
	 */
	struct ProfileEvent
	{
		/**
		 * This is a monotonically increasing number, unique to each event emitted
		 * in this run of the target application.
		 */
		uint32_t id;
				
		/**
		 * A reference to the enclosing event, if this is a stacked event.
		 */
		uint32_t parentEventRef;
		
		/**
		 * This is the monotonically increasing sample frame number.
		 */
		uint32_t frameNumber;

		/**
		 * This is the time of the event, in microseconds from the start of the 
		 * application profile.
		 */
		uint64_t timestamp;

		/**
		 * The type of the event.
		 */
		EventType eventType;
		
		/**
		 * The 32-bit value of the event.
		 */
		ProfileEventValue value;
		
		/**
		 * A reference to the human identifier of this event.
		 */
		uint32_t eventNameRef;
	};
	
	
	typedef moodycamel::ReaderWriterQueue<ProfileEvent> ProfileEventQueue;

}
#endif	/* EVENT_H */

