#include "profile.h"
#include "Context.h"
#include "Timing.h"

void esp_init()
{
	esp::_context = new esp::ProfileContext;
	esp_thread_init();
	esp::StartTimestampUpdate();
}

void esp_shutdown()
{
	esp::StopTimestampUpdate();
	esp::_context->JoinDrainThreadForShutdown();
	delete esp::_context;
	esp::_context = nullptr;
}

void esp_thread_init()
{
	esp::_context->InitThread();
}

void esp_frame_end()
{
	esp::_context->FrameEnd();
}

void esp_zone(const char *zoneName)
{
	esp::_thread_context->Zone(zoneName);
}

void esp_end()
{
	esp::_thread_context->End();
}

void esp_sample_int(const char *probeName, const int32_t& value)
{
	esp::_thread_context->Sample(probeName, value);
}

void esp_sample_uint(const char *probeName, const uint32_t& value)
{
	esp::_thread_context->Sample(probeName, value);
}

void esp_sample_float(const char *probeName, const float& value)
{
	esp::_thread_context->Sample(probeName, value);
}