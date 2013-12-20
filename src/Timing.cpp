#include "Timing.h"
using namespace esp;

std::atomic<int_fast64_t> esp::_current_timestamp(0);
bool esp::_run_timestamp_thread = true;
std::thread esp::_timerThread;

#define ESP_WINDOWS
#undef ESP_LINUX

#if (defined ESP_WINDOWS)

#include <Windows.h>

int64_t GetPerformanceCounterFrequency()
{
	LARGE_INTEGER queryQWORD;
	QueryPerformanceFrequency(&queryQWORD);
	return queryQWORD.QuadPart;
}

int64_t GetPerformanceCounterTime()
{
	LARGE_INTEGER queryQWORD;
	QueryPerformanceCounter(&queryQWORD);
	return queryQWORD.QuadPart;
}

void update_timestamp_thread(void)
{
	esp::_current_timestamp = 0;
	
	int64_t clockFrequency = GetPerformanceCounterFrequency();
	
	int64_t lastSampledTime;
	int64_t currentSampledTime;
	
	printf("Clock Frequency : %lld\n", clockFrequency);
	
	currentSampledTime = GetPerformanceCounterTime();
	
	while (esp::_run_timestamp_thread){
		lastSampledTime = currentSampledTime;
		currentSampledTime = GetPerformanceCounterTime();
		
		int64_t delta = currentSampledTime - lastSampledTime;
		delta *= 1000000;
		delta /= clockFrequency;
		
		//printf("Delta : %lld\n", delta);
		
		esp::_current_timestamp += delta;
		
		//std::this_thread::yield();
	}
}

#endif

#if (defined ESP_LINUX)

#include <time.h>
#include <stdio.h>

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

void update_timestamp_thread(void)
{
	struct timespec lastTimeSpec;
	struct timespec targetTimeSpec;
	
	clock_gettime(CLOCK_MONOTONIC, &lastTimeSpec);
	
	int64_t startTime = TimespecToLinearMicroseconds(lastTimeSpec);
	
	while (esp::_run_timestamp_thread)
	{
		clock_gettime(CLOCK_MONOTONIC, &lastTimeSpec);
		
		//current time in microseconds
		esp::_current_timestamp = TimespecToLinearMicroseconds(lastTimeSpec) - startTime;
	
		targetTimeSpec = lastTimeSpec;
		AddMicroseconds(&targetTimeSpec, 1); //2 microsecond resolution.
		
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &targetTimeSpec, nullptr);
	}
}

#endif

bool esp::StartTimestampUpdate()
{
	_run_timestamp_thread = true;
	_timerThread = std::thread(update_timestamp_thread);
	return true;
}

void esp::StopTimestampUpdate()
{
	_run_timestamp_thread = false;
}
