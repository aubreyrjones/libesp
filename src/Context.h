/* 
 * File:   Profile.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 2:55 PM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#if (defined ESP_LINUX)
#define ESP_TLS_DECL __thread
#elif (defined ESP_WINDOWS)
#define ESP_TLS_DECL __declspec( thread )
#endif

namespace esp
{

	class ThreadContext
	{
	};

	class ProfileContext
	{
	protected:
		int threadCount;
		
	};
	
	extern ESP_TLS_DECL ThreadContext* _thread_context;
	extern ProfileContext* _context;
}
#endif	/* CONTEXT_H */

