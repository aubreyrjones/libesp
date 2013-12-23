#include <ctime>
#include "profile.h"
#include "Context.h"
#include "Timing.h"
#include "EventStreamIO.h"
#include "ESPConfig.h"

static const int sessionNameBufferSize = 1024;

void esp_init(const char *sessionName, bool addTimestamp)
{
	char sessionNameBuffer[sessionNameBufferSize];
	std::time_t now = std::time(0);
	std::tm time;
#if (defined ESP_WINDOWS)
	localtime_s(&time, &now);
#elif (defined ESP_LINUX)
	localtime_r(&now, &time);
#endif

	char datestring[64];
	std::strftime(datestring, 64, "%y-%m-%d-%H%M", &time);
	
	if (!sessionName){
		snprintf(sessionNameBuffer, sessionNameBufferSize, "%s.esp", datestring);
	}
	else {
		if (addTimestamp){
			snprintf(sessionNameBuffer, sessionNameBufferSize, "%s-%s.esp", sessionName, datestring);
		}
		else {
			snprintf(sessionNameBuffer, sessionNameBufferSize, "%s.esp", sessionName);
		}
	}
	
	esp::_context = new esp::ProfileContext;
	esp_thread_init();
	esp::StartTimestampUpdate();
	esp::_context->StartDrain(new esp::RawEventWriter(sessionNameBuffer));
}


void esp_set_paused(bool shouldPause)
{
	esp::_paused = shouldPause;
}

bool esp_is_paused()
{
	return esp::_paused;
}

void esp_shutdown()
{
	esp::_context->PrintStatus();
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
	if (esp::_paused) return;
	
	if (!esp::_thread_context){
		esp_thread_init();
	}
	esp::_thread_context->Zone(zoneName);
}

void esp_end()
{
	if (!esp::_thread_context){
		esp_thread_init();
	}
	esp::_thread_context->End();
}

void esp_sample_int(const char *probeName, const int32_t& value)
{
	if (esp::_paused) return;
	
	if (!esp::_thread_context){
		esp_thread_init();
	}
	esp::_thread_context->Sample(probeName, value);
}

void esp_sample_uint(const char *probeName, const uint32_t& value)
{
	if (esp::_paused) return;
	
	if (!esp::_thread_context){
		esp_thread_init();
	}
	esp::_thread_context->Sample(probeName, value);
}

void esp_sample_float(const char *probeName, const float& value)
{
	if (esp::_paused) return;
	
	if (!esp::_thread_context){
		esp_thread_init();
	}
	esp::_thread_context->Sample(probeName, value);
}