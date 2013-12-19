/* 
 * File:   Event.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 1:38 PM
 */

#include <stdint.h>
#include "ESPConfig.h"
#include "queue/lockless_ring.h"
#ifndef EVENT_H
#define	EVENT_H

namespace esp
{
	enum EventType : uint32_t
	{
		EV_PROBE_UINT = 1,
		EV_PROBE_FLOAT = 2,
		EV_PROBE_INT = 3,
		EV_ZONE_INTERVAL = 1000,
		EV_NEW_STRING = 0xffffffff
	};
	
	/**
	 * A simple union to allow ProfileEventData to handle different types of 
	 * sampled data.
	 */
	union ProfileEventValue
	{
		float f;
		uint32_t ui;
		int32_t i;
	};
	
	struct ProfileEventHeader
	{
		/**
		 * The type of the event.
		 */
		EventType eventType;
		
		/**
		 * This is a monotonically increasing number, unique to each event emitted
		 * in this run of the target application.
		 * 
		 * ID numbers of EV_NEW_STRING events are unique only within the pool
		 * of strings--id numbers may overlap with regular profiler events.
		 */
		uint32_t id;		
	};
	
	/**
	 * This is the payload of profiler data captured in an event.
	 */
	struct ProfileEventData
	{
		
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
		int64_t timestamp;

		/**
		 * A reference to the human identifier of this event.
		 */
		uint32_t eventNameRef;		
		
		/**
		 * The 32-bit value of the event.
		 */
		ProfileEventValue value;		
	};

	/**
	 * A profile event is a single sample coming from within the target application.
	 */
	struct ProfileEvent
	{	
		ProfileEventHeader header;
		ProfileEventData data;
	};
	
	/**
	 * This is a string reference used internally by the profiler context to track
	 * strings that need to be streamed to the event consumer.
	 */
	struct RuntimeStringReference
	{
		//ID of the string
		uint32_t id;
		
		//Pointer to the string data.
		const char *ptr;
	};
	
	/**
	 * This is the header for a string stored to disk. In the raw stream file 
	 * format, this header is immediately followed by the non-null-terminated
	 * string contents.
	 */
	struct SerializedStringHeader
	{
		ProfileEventHeader header;
		uint32_t length;
	};
	
	typedef devious::LockessRingQueue<ProfileEvent, esp::espMaxEventBuffer> ProfileEventQueue;

}
#endif	/* EVENT_H */

