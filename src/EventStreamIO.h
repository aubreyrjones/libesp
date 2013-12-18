/* 
 * File:   EventStreamIO.h
 * Author: netzapper
 *
 * Created on December 17, 2013, 12:53 PM
 */

#include <stdio.h>

#include "Event.h"
#include "Comms.h"


#ifndef EVENTSTREAMIO_H
#define	EVENTSTREAMIO_H

namespace esp
{
	class RawEventWriter : public EventStreamConsumer
	{
	private:
		FILE *outputFile;
		ProfileEvent writeBuffer[espWriteBufferSize];
		int writeBufferUsed;
		
	protected:
		virtual void Drain();
	public:
		RawEventWriter(const char *path);
		virtual ~RawEventWriter();
		
	};
}

#endif	/* EVENTSTREAMIO_H */

