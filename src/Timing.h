/* 
 * File:   Timing.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 5:21 PM
 */


#include <stdint.h>


#ifndef TIMING_H
#define	TIMING_H

namespace esp
{
	/**
	 * Returns the highest-resolution current time possible.
	 * 
     * @return The current time in microseconds.
     */
	uint64_t GetSystemTimestamp();
}	

#endif	/* TIMING_H */

