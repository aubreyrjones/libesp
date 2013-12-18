#include <stdio.h>
#include "profile.h"
#include "Context.h"

#include <chrono>
#include <thread>

void SimpleFunction()
{
	for (int j = 0; j < 5; j++) {
		esp_zone("inner");
		for (int k = 0; k < 10; k++) {
			printf("%i,", j * k);
		}
		esp_end();
	}
	printf(".\n");
}

int main(int argc, char **argv)
{
	esp_init();
	
	for (int i = 0; i < 1000; i++){	
		esp_zone("outer");
		SimpleFunction();
		esp_end();
		
		esp_frame_end();
	}
	
	esp_shutdown();
}