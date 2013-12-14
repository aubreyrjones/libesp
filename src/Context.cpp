#include "Context.h"
#include "Timing.h"
using namespace esp;

static ESP_TLS_DECL ThreadContext* _thread_context = nullptr;
static ProfileContext* _context = nullptr;

void ThreadContext::Zone(const char *zoneName)
{
	ProfileEvent *parentZone = profileIntervalStack.Peek();
	ProfileEvent *ev = profileIntervalStack.Push();
	ev->eventType = EV_ZONE_INTERVAL;
	ev->timestamp = GetSystemTimestamp();
	ev->frameNumber = frameNumber;
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
