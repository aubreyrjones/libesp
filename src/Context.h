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

#if (defined ESP_LINUX)
#define ESP_TLS_DECL __thread
#elif (defined ESP_WINDOWS)
#define ESP_TLS_DECL __declspec( thread )
#endif

namespace esp
{
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
		ProfileEventQueue pendingEvents;
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
		
	};
	
	
	extern ESP_TLS_DECL ThreadContext* _thread_context;
	extern ProfileContext* _context;
}
#endif	/* CONTEXT_H */

