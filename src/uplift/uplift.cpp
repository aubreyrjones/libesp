#include <stdio.h>

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
	
	sfs.StartBulkInsert();
	while (fread(&pe, sizeof(pe), 1, inFile))
	{
		sfs.WriteEvent(pe);
	}
	sfs.EndBulkInsert();
	
	fclose(inFile);
	
	return 0;
}
