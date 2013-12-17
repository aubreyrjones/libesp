#include "Context.h"
#include "Timing.h"
#include <chrono>

using namespace esp;

ESP_TLS_DECL ThreadContext* esp::_thread_context = nullptr;
ProfileContext* esp::_context = nullptr;

ThreadContext::ThreadContext(int32_t threadIndex) :
	threadIndex(threadIndex),
	profileIntervalStack(espMaxZoneRecursion)
{
	
}

void ThreadContext::Zone(const char *zoneName)
{
	if (!_context){
		return;
	}
	
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
	else {
		ev->parentEventRef = 0;
	}
}

void ThreadContext::End()
{
	if (!_context){
		return;
	}
	
	ProfileEvent *ev = profileIntervalStack.Pop();
	if (!ev){
		return;
	}
	
	int64_t curtime = esp::_current_timestamp;
	ev->value.ui = curtime - ev->timestamp;
	
	if (!_context->eventQueue.TryEnqueue(*ev)){
		printf("\n.over.\n");
	}
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

void drain_bounce(ProfileContext *pc)
{
	pc->DrainEvents();
}

ProfileContext::ProfileContext() : 
	threadCount(0), 
	nextMessageID(0),
	frameNumber(0),
	runDrainThread(true)
{
	for (int i = 0; i < espMaxThreadCount; i++){
		new (threadContexts + i) ThreadContext(i);
	}
	
	eventConsumer = new EventStreamConsumer; //null event consumer
	drainThread = new std::thread(drain_bounce, this);
}

ProfileContext::~ProfileContext()
{
	if (eventConsumer){
		delete eventConsumer;
	}
}

void ProfileContext::InitThread()
{
	if (!_thread_context){
		int threadIndex = threadCount.fetch_add(1);
		_thread_context = &threadContexts[threadIndex];
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

void ProfileContext::DrainEvents()
{
	bool hadEventsLast = false;
	while (runDrainThread || hadEventsLast) {
		hadEventsLast = false;
		ProfileEvent ev;
		while (eventQueue.TryDequeue(&ev)) {
			hadEventsLast = true;
			eventConsumer->WriteEvent(ev);
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void ProfileContext::JoinDrainThreadForShutdown()
{
	if (drainThread){
		runDrainThread = false;
		drainThread->join();
	}
}