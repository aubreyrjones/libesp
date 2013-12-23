#include "Context.h"
#include "Timing.h"
#include <chrono>

using namespace esp;

ESP_TLS_DECL ThreadContext* esp::_thread_context = nullptr;
ProfileContext* esp::_context = nullptr;
bool esp::_paused = false;

ThreadContext::ThreadContext(int32_t threadIndex) :
	threadIndex(threadIndex),
	profileIntervalStack(espMaxZoneRecursion)
{
	
}

uint32_t ThreadContext::PeekParentID()
{
	ProfileEvent *parentZone = profileIntervalStack.Peek();
	if (parentZone){
		return parentZone->header.id;
	}
	else {
		return 0;
	}
}

ProfileEvent* ThreadContext::StampEvent(ProfileEvent *ev, const char *nameString)
{
	if (!ev){
		return nullptr;
	}
	
	ev->header.id = _context->NextEventID();
	ev->data.threadID = threadIndex;
	ev->data.timestamp = esp::_current_timestamp;
	ev->data.frameNumber = _context->GetFrameNumber();
	ev->data.eventNameRef = _context->MapStringToReference(nameString);
	
	return ev;
}


void ThreadContext::Zone(const char *zoneName)
{
	if (!_context){
		return;
	}
	
	uint32_t parentRef = PeekParentID();
	ProfileEvent *ev = StampEvent(profileIntervalStack.Push(), zoneName);
	if (!ev) {
		return;
	}
	
	ev->header.eventType = EV_ZONE_INTERVAL;
	ev->data.parentEventRef = parentRef;
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
	
	ev->data.value.ui = esp::_current_timestamp - ev->data.timestamp;
	
	_context->eventQueue.TryEnqueue(*ev);
}

void ThreadContext::Sample(const char *probeName, const int32_t& value)
{
	if (!_context){
		return;
	}
	
	ProfileEvent pe;
	StampEvent(&pe, probeName);
	pe.header.eventType = EV_PROBE_INT;
	pe.data.value.i = value;
	pe.data.parentEventRef = PeekParentID();
	
	_context->eventQueue.TryEnqueue(pe);
}

void ThreadContext::Sample(const char *probeName, const uint32_t& value)
{
	if (!_context){
		return;
	}
	
	ProfileEvent pe;
	StampEvent(&pe, probeName);
	pe.header.eventType = EV_PROBE_UINT;
	pe.data.value.ui = value;
	pe.data.parentEventRef = PeekParentID();
	
	_context->eventQueue.TryEnqueue(pe);	
}

void ThreadContext::Sample(const char *probeName, const float& value)
{
	if (!_context){
		return;
	}
	
	ProfileEvent pe;
	StampEvent(&pe, probeName);
	pe.header.eventType = EV_PROBE_FLOAT;
	pe.data.value.f = value;
	pe.data.parentEventRef = PeekParentID();
	
	_context->eventQueue.TryEnqueue(pe);	
}


//==================ProfileContext===================

ProfileContext::ProfileContext() : 
	threadCount(0), 
	nextMessageID(1),
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
	
	if (!eventConsumer->GetStringQueue()->TryEnqueue(stringRef)){
		printf("String overflow.\n");
	}
	
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


