#include <stdio.h>
#include "profile.h"

void SimpleFunction()
{
	for (int j = 0; j < 10; j++) {
		for (int k = 0; k < 10; k++) {
			printf("%i,", j * k);
		}
	}
	printf(".\n");
}

int main(int argc, char **argv)
{
	esp_init();
	
	for (int i = 0; i < 1000; i++){	
		esp_zone("test_zone");
		SimpleFunction();
		esp_end();
		
		esp_frame_end();
	}
	
	esp_shutdown();
}