#include "Context.h"
#include "Timing.h"
using namespace esp;

ESP_TLS_DECL ThreadContext* esp::_thread_context = nullptr;
ProfileContext* esp::_context = nullptr;

ThreadContext::ThreadContext(int32_t threadIndex) :
	threadIndex(threadIndex),
	pendingEvents(espMaxEventBuffer),
	profileIntervalStack(espMaxZoneRecursion)
{
	
}

void ThreadContext::Zone(const char *zoneName)
{
	ProfileEvent *parentZone = profileIntervalStack.Peek();
	ProfileEvent *ev = profileIntervalStack.Push();
	if (!ev) {
		return;
	}
	ev->id = _context->NextEventID();
	ev->eventType = EV_ZONE_INTERVAL;
	ev->timestamp = esp::_current_timestamp;
	ev->frameNumber = _context->GetFrameNumber();
	ev->eventNameRef = _context->MapStringToReference(zoneName);
	if (parentZone){
		ev->parentEventRef = parentZone->id;
	}
}

void ThreadContext::End()
{
	ProfileEvent *ev = profileIntervalStack.Pop();
	if (!ev){
		return;
	}
	
	int64_t curtime = esp::_current_timestamp;
	ev->value.ui = curtime - ev->timestamp;
	
	pendingEvents.enqueue(*ev);
}

void ThreadContext::Sample(const char *probeName, const int32_t& value)
{
	
}

void ThreadContext::Sample(const char *probeName, const uint32_t& value)
{
	
}

void ThreadContext::Sample(const char *probeName, const float& value)
{
	
}


//==================ProfileContext===================

ProfileContext::ProfileContext() : 
	threadCount(0), 
	nextMessageID(0),
	frameNumber(0),
	runDrainThread(true),
	eventMutex(),
	eventCondition()
{
	
}

void ProfileContext::InitThread()
{
	if (!_thread_context){
		int threadIndex = threadCount.fetch_add(1);
		_thread_context = threadContexts[threadIndex] = new ThreadContext(threadIndex);
	}
	
}

void ProfileContext::FrameEnd()
{
	++frameNumber;
	frameTimestamp = _current_timestamp;
}


uint32_t ProfileContext::MapStringToReference(const char* string)
{
	return 0;
}

uint32_t ProfileContext::NextEventID()
{
	return nextMessageID.fetch_add(1);
}

void ProfileContext::NotifyEvent()
{
	std::lock_guard<std::mutex> eventLock(eventMutex);
	eventCondition.notify_one();
}

void ProfileContext::DrainEvents()
{
	while (runDrainThread) {
		for (int i = 0; i < threadCount; i++) {
			ProfileEvent ev;
			while (threadContexts[i]->pendingEvents.try_dequeue(ev)) {
				eventConsumer->WriteEvent(ev);
			}
		}
		std::unique_lock<std::mutex> eventLock(eventMutex);
		eventCondition.wait(eventLock);
	}
}