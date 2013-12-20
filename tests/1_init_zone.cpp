#include <stdio.h>
#include "profile.h"
#include "Context.h"

#include <chrono>
#include <thread>

void SimpleFunction()
{
	for (int j = 0; j < 50; j++) {
		esp_zone("inner");
		for (int k = 0; k < 100; k++) {
			//printf("%i,", j * k);
		}
		esp_end();
	}
	//printf(".\n");
}

int main(int argc, char **argv)
{
	esp_init("test1", false);
	
	for (int i = 0; i < 1000; i++){	
		esp_zone("outer");
		//SimpleFunction();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		esp_end();
		
		esp_frame_end();
	}
	
	esp_shutdown();
	
	return 0;
}