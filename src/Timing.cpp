#include "Timing.h"
#include <time.h>
using namespace esp;

#if (defined ESP_LINUX)

	uint64_t GetSystemTimestamp()
	{
		struct timespec time;
		clock_gettime(CLOCK_MONOTONIC, &time);
		
		uint64_t linearTime = time.tv_sec * 1000000; //seconds to microseconds
		linearTime += time.tv_nsec / 1000; //nanoseconds to microseconds
		
		return linearTime;
	}

#endif
