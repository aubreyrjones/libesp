/* 
 * File:   Timing.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 5:21 PM
 */


#include <stdint.h>

#if (defined ESP_LINUX)
#include <pthread.h>
#endif

#ifndef TIMING_H
#define	TIMING_H

namespace esp
{
	bool StartTimestampUpdate();

	void StopTimestampUpdate();
	
	extern uint64_t _current_timestamp;
}	

#endif	/* TIMING_H */

