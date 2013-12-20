#include <stdio.h>
#include "profile.h"
#include "Context.h"

#include <chrono>
#include <thread>

void ThreadedFunction()
{
	esp_thread_init();
	
	for (int i = 0; i < 1000; i++){
		esp_zone("thread_outer");
		for (int k = 0; k < 10; k++){
			esp_sample_int("k_counter", k);
		}
		esp_end();
	}
}

int main(int argc, char **argv)
{
	esp_init("test2", false);
	
	std::thread threadA(ThreadedFunction), threadB(ThreadedFunction);
	
	for (int i = 0; i < 1000; i++){	
		esp_zone("main_thread");
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		esp_end();
		
		esp_frame_end();
	}
	
	threadA.join();
	threadB.join();
	
	esp_shutdown();
	
	return 0;
}