#include "Context.h"
using namespace esp;

static ESP_TLS_DECL ThreadContext* _thread_context = nullptr;
static ProfileContext* _context = nullptr;

