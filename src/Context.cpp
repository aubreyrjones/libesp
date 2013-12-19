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
	ev->header.id = _context->NextEventID();
	ev->header.eventType = EV_ZONE_INTERVAL;
	ev->data.timestamp = esp::_current_timestamp;
	ev->data.frameNumber = _context->GetFrameNumber();
	ev->data.eventNameRef = _context->MapStringToReference(zoneName);
	if (parentZone){
		ev->data.parentEventRef = parentZone->header.id;
	}
	else {
		ev->data.parentEventRef = 0;
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
	ev->data.value.ui = curtime - ev->data.timestamp;
	
	if (!_context->eventQueue.TryEnqueue(*ev)){
		printf("\n.overflow.\n");
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

ProfileContext::ProfileContext() : 
	threadCount(0), 
	nextMessageID(0),
	frameNumber(0)
{
	for (int i = 0; i < espMaxThreadCount; i++){
		new (threadContexts + i) ThreadContext(i);
	}
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
	int index = strings.Get(string);
	if (index >= 0){
		return index;
	}
	
	index = strings.Put(string);
	
	if (index < 0){
		return -1; //error
	}
	
	RuntimeStringReference stringRef;
	stringRef.id = index;
	stringRef.ptr = string;
	
	eventConsumer->GetStringQueue()->TryEnqueue(stringRef);
	
	return index;
}

uint32_t ProfileContext::NextEventID()
{
	return nextMessageID.fetch_add(1, std::memory_order_acq_rel);
}

void ProfileContext::JoinDrainThreadForShutdown()
{
	printf("Shutting down ESP.\n");
	eventConsumer->ShutdownAndWait();
}

void ProfileContext::StartDrain(EventStreamConsumer *consumer)
{
	eventConsumer = consumer;
	eventConsumer->RegisterEventQueue(&eventQueue);
	eventConsumer->StartDrainThread();
}

void ProfileContext::PrintStatus()
{
	printf("Outstanding events: %d\n", eventQueue.GetSize());
}


