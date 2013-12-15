/* 
 * File:   Profile.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 2:55 PM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <stdint.h>
#include <atomic>

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
		
	public:
		
		ThreadContext(int32_t threadIndex);
		
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
		std::atomic_uint_fast32_t threadCount;
		
		std::atomic_uint_fast32_t nextMessageID;
	
		std::atomic_uint_fast32_t frameNumber;
		
		std::atomic_uint_fast64_t frameTimestamp;
	public:
		
		/**
		 * Create a new profile context.
		 * 
		 * This should not be called directly, in most circumstances.
         */
		ProfileContext();
		
		/**
		 * This should be called once per thread to initialize its ThreadContext.
		 * 
		 * This method has no effect if the thread has already been initialized,
		 * so it is safe to call this method more than once per thread. However,
		 * this may be a slight performance penalty.
         */
		void InitThread();
		
		/**
		 * Signifies the end of a new profiling frame.
         */
		void FrameEnd();
		
		/**
		 * Maps a constant string to a reference number which can be used to
		 * look up the string from the database.
         * @param string the constant string to map.
         * @return the reference number for the string.
         */
		uint32_t MapStringToReference(const char* string);
		
		/**
		 * Returns the next event ID and increments the internal counter.
         * @return the next available sequential event id.
         */
		uint32_t NextEventID();
		
		/**
		 * Gets the current frame number.
         * @return 
         */
		uint32_t GetFrameNumber() const
		{
			return frameNumber;
		}
	};
	
	
	extern ESP_TLS_DECL ThreadContext* _thread_context;
	extern ProfileContext* _context;
}
#endif	/* CONTEXT_H */

