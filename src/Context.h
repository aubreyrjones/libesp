/* 
 * File:   Profile.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 2:55 PM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <stdint.h>

#include "Event.h"
#include "stack/slabstack.h"

#if (defined ESP_LINUX)
#define ESP_TLS_DECL __thread
#elif (defined ESP_WINDOWS)
#define ESP_TLS_DECL __declspec( thread )
#endif

namespace esp
{
	typedef devious::SlabStack<ProfileEvent> ProfileEventStack;
	
	/**
	 * This is the context for a thread of execution.
	 */
	class ThreadContext
	{
		friend class ProfileContext;
		
	protected:
		/**
		 * The ESP index of this thread, determined by the order in which
		 * the threads call esp_thread_init().
		 */
		int32_t threadIndex;
		/**
		 * Events that we want the main thread to stream to storage.
		 */
		ProfileEventQueue pendingEvents;
		
		/**
		 * The stack of profile intervals.
		 */
		ProfileEventStack profileIntervalStack;
		
		uint32_t frameNumber;
		
	public:
		
		void Zone(const char *zoneName);
		void End();
		void Sample(const char *probeName, const int32_t& value);
		void Sample(const char *probeName, const uint32_t& value);
		void Sample(const char *probeName, const float& value);
	};

	
	
	/**
	 * This is the overall context for the entire ESP profiler session.
	 */
	class ProfileContext
	{
	protected:
		/**
		 * How many different threads have called esp_thread_init()?
		 */
		int threadCount;
	
	public:
		
		uint32_t MapStringToReference(const char* string);
	};
	
	
	extern ESP_TLS_DECL ThreadContext* _thread_context;
	extern ProfileContext* _context;
}
#endif	/* CONTEXT_H */

