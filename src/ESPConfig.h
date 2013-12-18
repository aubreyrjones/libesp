/* 
 * File:   ESPConfig.h
 * Author: aubrey
 *
 * Created on December 16, 2013, 5:51 PM
 */

#ifndef ESPCONFIG_H
#define	ESPCONFIG_H

namespace esp {

	enum {
		//Maximum number of concurrent threads.
		espMaxThreadCount = 64,
		
		//Maximum depth of zone recursion.
		espMaxZoneRecursion = 1024,
		
		//The size of the in-memory event buffer, measured in events. If you're 
		//dropping events, consider increasing this.
		espMaxEventBuffer = 1024 * 512,
		
		//The size of the write block buffer, in number of events.
		espWriteBufferSize = 16384,
		
		//If the IO system has accumulated >= espWriteBufferSize - espWriteBufferThreshold,
		//it will attempt a write to disk.
		espWriteBufferThreshold = 2048,
		espWriteBufferThresholdPrecompute = espWriteBufferSize - espWriteBufferThreshold,
	};
}

#endif	/* ESPCONFIG_H */

