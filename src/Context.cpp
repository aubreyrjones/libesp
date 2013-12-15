#include "Context.h"
#include "Timing.h"
using namespace esp;

static ESP_TLS_DECL ThreadContext* _thread_context = nullptr;
static ProfileContext* _context = nullptr;

ThreadContext::ThreadContext(int32_t threadIndex) :
	threadIndex(threadIndex),
	pendingEvents(1024),
	profileIntervalStack(1024)
{
	
}

void ThreadContext::Zone(const char *zoneName)
{
	ProfileEvent *parentZone = profileIntervalStack.Peek();
	ProfileEvent *ev = profileIntervalStack.Push();
	if (!ev) {
		return;
	}
	ev->id = _context->NextMessageID();
	ev->eventType = EV_ZONE_INTERVAL;
	ev->timestamp = GetSystemTimestamp();
	ev->frameNumber = _context->GetFrameNumber();
	ev->eventNameRef = _context->MapStringToReference(zoneName);
	if (parentZone){
		ev->parentEventRef = parentZone->id;
	}
}

void ThreadContext::End()
{
	
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
	nextMessageID(0)
{
	
}


ThreadContext* ProfileContext::NewThreadContext()
{
	ThreadContext *newContext = new ThreadContext(threadCount.fetch_add(1));
}


uint32_t ProfileContext::NextMessageID()
{
	return nextMessageID.fetch_add(1);
}