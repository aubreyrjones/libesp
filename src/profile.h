/* 
 * File:   profile.h
 * Author: aubrey
 *
 * Created on December 13, 2013, 3:21 PM
 */

#include <stdint.h>

#ifndef PROFILE_H
#define	PROFILE_H

extern "C"
{
	/**
	 * Initialize the overall ESP context.
	 * 
	 * This also initializes the main thread's context.
	 * 
	 * sessionName is the name of the session to create. If a session name is given,
	 * and addTimestamp is false, then the created session will simply be called
	 * "sessionName.esp". If addTimestamp is true, then the session name will
	 * be in the format "sessionName-YY-MM-DD-hhmm.esp".
	 * 
	 * If no sessionName is given, then the session name will simply be the 
	 * timestamp "YY-MM-DD-hhmm.esp", regardless of the setting to addTimestamp.
	 */
	void esp_init(const char *sessionName, bool addTimestamp);

	/**
	 * Shut down the overall ESP context and disconnect from servers or
	 * local storage.
	 */
	void esp_shutdown();

	/**
	 * Call this at least once in each thread before submitting zones or
	 * probes.
	 * 
	 * This function is idempotent, meaning that you may freely call it
	 * multiple times in each thread without any additional effect on the
	 * state of the system. There may (will) be performance penalties to doing
	 * so, however. In general, strive to call this only once for
	 * each thread created.
	 */
	void esp_thread_init();
	
	/**
	 * Call this once per "frame" (whatever that means in your context), at
	 * the end of the frame.
     */
	void esp_frame_end();

	/**
	 * Starts a new profiling zone by pushing it onto the interval stack.
	 * 
	 * @param zoneName This should be a string literal, const
	 * variable, or a #define.
	 * 
	 * The identity of the string pointer itself is used to compare
	 * identity of zones.
	 */
	void esp_zone(const char *zoneName);

	/**
	 * Ends the last declared profiling zone by popping the top of the 
	 * interval stack and emitting the corresponding ZONE_INTERVAL event.
	 */
	void esp_end();

	/**
	 * Sample any given value by inserting a PROBE_* event into the 
	 * queue.
	 * @param probeName The name of the probe. Pointer identity is used to identify
	 * probes, so use a string literal or other consolidated string.
	 * @param value The value of the probe to 
	 */
	void esp_sample_int(const char *probeName, const int32_t& value);
	void esp_sample_uint(const char *probeName, const uint32_t& value);
	void esp_sample_float(const char *probeName, const float& value);
}

#endif	/* PROFILE_H */

