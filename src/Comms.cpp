#include "Comms.h"
using namespace esp;

void EventStreamConsumer::DrainThreadBounce(EventStreamConsumer *consumer)
{
	consumer->DrainLoop();
}

void EventStreamConsumer::DrainLoop()
{
	while (runThread){
		Drain();
	}
}

void EventStreamConsumer::RegisterEventQueue(ProfileEventQueue *queue)
{
	contextQueue = queue;
}

void EventStreamConsumer::StartDrainThread()
{
	if (!drainThread){
		drainThread = new std::thread(DrainThreadBounce, this);
	}
}

void EventStreamConsumer::StopDrainThread()
{
	runThread = false;
}