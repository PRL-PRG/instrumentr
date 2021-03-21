#ifndef INSTRUMENTR_TRACER_H
#define INSTRUMENTR_TRACER_H

#include <instrumentr/api.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

INSTRUMENTR_OBJECT_INTEROP_DECLARE_API(tracer, INSTRUMENTR_OBJECT_TYPE_TRACER)

void instrumentr_tracer_initialize_tracing(instrumentr_tracer_t tracer,
                                           const char* working_directory,
                                           SEXP r_code,
                                           SEXP r_environment);

SEXP instrumentr_tracer_finalize_tracing(instrumentr_tracer_t tracer);

void instrumentr_tracer_clear(instrumentr_tracer_t tracer);

void instrumentr_tracer_reset(instrumentr_tracer_t tracer);

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer);

void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback);

void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer);

void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status);

INSTRUMENTR_TRACER_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_TRACER_H */
