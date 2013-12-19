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

void RawEventWriter::WriteString(RuntimeStringReference& string)
{
	SerializedStringHeader header;
	header.header.id = string.id;
	header.header.eventType = EV_NEW_STRING;
	header.length = strlen(string.ptr);
	
	fwrite(&header, sizeof(SerializedStringHeader), 1, outputFile);
	fwrite(string.ptr, sizeof(char), header.length, outputFile);
}

void RawEventWriter::FlushStrings()
{
	RuntimeStringReference rsr;
	
	while (stringQueue.TryDequeue(&rsr)){
		WriteString(rsr);
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
	
	FlushStrings();
}

void RawEventWriter::Drain()
{
	
	writeBufferUsed += contextQueue->TryDequeue(writeBuffer + writeBufferUsed, espWriteBufferSize - writeBufferUsed);

	if (writeBufferUsed >= espWriteBufferThresholdPrecompute){
		Flush();
	}
	
	FlushStrings();
	
	stillBusy = !(contextQueue->Empty() && stringQueue.Empty());
	std::this_thread::yield();
}
