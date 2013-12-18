#include "Comms.h"
using namespace esp;

void EventStreamConsumer::DrainThreadBounce(EventStreamConsumer *consumer)
{
	consumer->DrainLoop();
}

EventStreamConsumer::~EventStreamConsumer()
{
}

void EventStreamConsumer::DrainLoop()
{
	while (runThread || stillBusy){
		Drain();
	}
	Flush();
}

void EventStreamConsumer::RegisterEventQueue(ProfileEventQueue *queue)
{
	contextQueue = queue;
}

void EventStreamConsumer::StartDrainThread()
{
	if (!runThread){
		runThread = true;
		drainThread = std::thread(DrainThreadBounce, this);
	}
}

void EventStreamConsumer::ShutdownAndWait()
{
	if (runThread){
		runThread = false;
		if (drainThread.joinable()){
			drainThread.join();
		}
	}
}