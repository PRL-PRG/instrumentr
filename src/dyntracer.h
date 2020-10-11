#ifndef INSTRUMENTR_DYNTRACER_H
#define INSTRUMENTR_DYNTRACER_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/tracer.h>

#ifdef USING_DYNTRACE

dyntracer_t* instrumentr_dyntracer_create(instrumentr_tracer_t tracer);

void instrumentr_dyntracer_destroy(dyntracer_t* dyntracer);

instrumentr_tracer_t instrumentr_dyntracer_get_tracer(dyntracer_t* dyntracer);

#endif

#endif /* INSTRUMENTR_DYNTRACER_H */
