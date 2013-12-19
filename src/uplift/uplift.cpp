#include <stdio.h>
#include <stdlib.h>

#include "Event.h"
#include "FileStore.h"

bool ReadRecord(FILE *in, esp::SessionFileStore *store)
{
	char stringBuffer[4096];
	
	esp::ProfileEventHeader header;
	esp::ProfileEvent pe;
	esp::SerializedStringHeader ssh;
	
	if (fread(&header, sizeof(header), 1, in)){
		switch (header.eventType)
		{
		case esp::EV_NEW_STRING:
			ssh.header = header;
			if (fread(&ssh.length, sizeof(ssh.length), 1, in)){
				int read = fread(stringBuffer, sizeof(char), ssh.length, in);
				stringBuffer[read] = '\0';
				store->WriteString(ssh.header.id, stringBuffer);
				return true;
			}
			else {
				printf("Cannot read string length.\n");
				return false;
			}
		default:
			pe.header = header;
			if (fread(&pe.data, sizeof(pe.data), 1, in)){
				store->WriteEvent(pe);
				return true;
			}
			else {
				printf("Cannot read event data.\n");
				return false;
			}
		}
	}
	
	return false;
}

int main(int argc, char **argv)
{
	if (argc < 2){
		printf("USAGE: esp_uplift SESSION_FILE\n");
	}
	
	esp::SessionFileStore sfs("session_stream.sqlite");
	sfs.Initialize();
	
	esp::ProfileEvent pe;
	
	FILE* inFile = fopen(argv[1], "rb");
	
	if (!inFile){
		printf("Cannot open file: %s\n", argv[1]);
		exit(2);
	}
	
	sfs.StartBulkInsert();
	while (ReadRecord(inFile, &sfs));
	sfs.EndBulkInsert();
	
	fclose(inFile);
	
	return 0;
}
