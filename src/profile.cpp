#include "profile.h"
#include "Context.h"

void esp_init()
{
	esp::_context = new esp::ProfileContext;
	esp_thread_init();
}

void esp_shutdown()
{
	//I don't know what this does yet.
}

void esp_thread_init()
{
	esp::_context->InitThread();
}

void esp_frame_end()
{
	esp::_context->FrameEnd();
}
