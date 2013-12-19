#include <stdio.h>
#include <stdlib.h>

#include "Event.h"
#include "FileStore.h"

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
	while (fread(&pe, sizeof(pe), 1, inFile))
	{
		sfs.WriteEvent(pe);
	}
	sfs.EndBulkInsert();
	
	fclose(inFile);
	
	return 0;
}
