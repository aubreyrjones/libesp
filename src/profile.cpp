#include "profile.h"
#include "Context.h"

void esp_init()
{
	esp::_context = new esp::ProfileContext;
	esp_thread_init();
}


void esp_thread_init()
{
	if (!esp::_thread_context){
		esp::_thread_context = esp::_context->NewThreadContext();
	}
}