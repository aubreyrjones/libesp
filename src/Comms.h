/* 
 * File:   Comms.h
 * Author: aubrey
 *
 * Created on December 16, 2013, 5:35 PM
 */
#include <thread>
#include <atomic>
#include "Event.h"

#ifndef COMMS_H
#define	COMMS_H

namespace esp
{
	
	
	class EventStreamConsumer
	{
	protected:
		ProfileEventQueue *contextQueue;
		std::thread drainThread;
		std::atomic_bool runThread;
		std::atomic_bool stillBusy;
		
		static void DrainThreadBounce(EventStreamConsumer *consumer);
		
	protected:
		virtual void Drain() = 0;
		virtual void Flush() = 0;
		
		void DrainLoop();
	public:
		EventStreamConsumer() : contextQueue(nullptr), drainThread(), runThread(false), stillBusy(false) {};
		virtual ~EventStreamConsumer();
		
		void RegisterEventQueue(ProfileEventQueue *queue);
		void StartDrainThread();
		
		void ShutdownAndWait();
	};
}

#endif	/* COMMS_H */

