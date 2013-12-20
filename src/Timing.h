/* 
 * File:   Timing.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 5:21 PM
 */


#include <stdint.h>
#include <atomic>
#include <thread>
#include <stdint.h>

#ifndef TIMING_H
#define	TIMING_H

namespace esp
{
	bool StartTimestampUpdate();

	void StopTimestampUpdate();
	
	extern std::atomic<int_fast64_t> _current_timestamp;
	extern bool _run_timestamp_thread;
	extern std::thread _timerThread;

}	

#endif	/* TIMING_H */

