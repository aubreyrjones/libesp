#include "EventStreamIO.h"

using namespace esp;

RawEventWriter::RawEventWriter(const char* path) : 
	EventStreamConsumer(),
	outputFile(nullptr),
	writeBufferUsed(0)
{
	outputFile = fopen(path, "wb");
	if (!outputFile){
		perror("Cannot open session stream file");
	}
}


void RawEventWriter::Drain()
{
	writeBufferUsed += contextQueue->TryDequeue(writeBuffer, espWriteBufferSize - writeBufferUsed);
	if (writeBufferUsed >= espWriteBufferThresholdPrecompute){
		int toWrite = writeBufferUsed;
		int written = 0;
		do {
			int thisWrite = fwrite(writeBuffer + written, sizeof(ProfileEvent), toWrite, outputFile);
			written += thisWrite;
			toWrite -= thisWrite;
		} while(toWrite);
	}
}