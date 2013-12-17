#include "Timing.h"
using namespace esp;

int64_t esp::_current_timestamp = 0;

#if (defined ESP_LINUX)

#include <time.h>
#include <stdio.h>

static pthread_t _timestamp_update_thread;
static bool _run_thread = true;

int64_t TimespecToLinearMicroseconds(const struct timespec& spec)
{
	return (spec.tv_sec * 1000000) + (spec.tv_nsec / 1000);
}

void AddMicroseconds(struct timespec *spec, int nMicroseconds)
{
	const long ONE_BILLION = 1000000000; 
	spec->tv_nsec += nMicroseconds * 1000;
	while (spec->tv_nsec > ONE_BILLION)
	{
		spec->tv_sec++;
		spec->tv_nsec -= ONE_BILLION;
	}
}

void* update_timestamp_thread(void *)
{

	struct timespec lastTimeSpec;
	struct timespec targetTimeSpec;
	
	clock_gettime(CLOCK_MONOTONIC, &lastTimeSpec);
	
	int64_t startTime = TimespecToLinearMicroseconds(lastTimeSpec);
	
	while (_run_thread)
	{
		clock_gettime(CLOCK_MONOTONIC, &lastTimeSpec);
		
		//current time in microseconds
		_current_timestamp = TimespecToLinearMicroseconds(lastTimeSpec) - startTime;
	
		targetTimeSpec = lastTimeSpec;
		AddMicroseconds(&targetTimeSpec, 2); //2 microsecond resolution.
		
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &targetTimeSpec, nullptr);
	}
}

bool esp::StartTimestampUpdate()
{
	int status;
	
	status = pthread_create(&_timestamp_update_thread, nullptr, update_timestamp_thread, nullptr);
	if (status){
		perror("Cannot start timing thread");
		return false;
	}
	
	return true;
}

void esp::StopTimestampUpdate()
{
	_run_thread = false;
}

#endif
