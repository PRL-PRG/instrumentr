#ifndef INSTRUMENTR_DYNTRACE_H
#define INSTRUMENTR_DYNTRACE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer);

void instrumentr_dyntracer_destroy(dyntracer_t* dyntracer);

instrumentr_tracer_t instrumentr_dyntracer_get_tracer(dyntracer_t* dyntracer);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_DYNTRACE_H */
