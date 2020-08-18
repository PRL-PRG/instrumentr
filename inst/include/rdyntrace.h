#include "Rincludes.h"
#include "Context.hpp"

#ifdef USING_DYNTRACE

extern "C" {
dyntracer_t* rdyntrace_create_dyntracer();
void rdyntrace_destroy_dyntracer(dyntracer_t* dyntracer);
}

#endif /* USING_DYNTRACE */
