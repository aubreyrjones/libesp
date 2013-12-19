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

RawEventWriter::~RawEventWriter()
{
	if (outputFile){
		fflush(outputFile);
		fclose(outputFile);
	}
}

void RawEventWriter::Flush()
{
	int toWrite = writeBufferUsed;
	int written = 0;
	do {
		int thisWrite = fwrite(writeBuffer + written, sizeof (ProfileEvent), toWrite, outputFile);
		written += thisWrite;
		toWrite -= thisWrite;
	}
	while (toWrite > 0);

	writeBufferUsed = 0;	
}

void RawEventWriter::Drain()
{
	
	writeBufferUsed += contextQueue->TryDequeue(writeBuffer + writeBufferUsed, espWriteBufferSize - writeBufferUsed);

	if (writeBufferUsed >= espWriteBufferThresholdPrecompute){
		Flush();
	}
	
	stillBusy = !contextQueue->Empty();
	std::this_thread::yield();
}