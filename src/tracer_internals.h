#ifndef INSTRUMENTR_TRACER_INTERNALS_H
#define INSTRUMENTR_TRACER_INTERNALS_H

#include <instrumentr/tracer.h>

/********************************************************************************
 * create
 *******************************************************************************/

SEXP r_instrumentr_tracer_create();

#ifdef USING_DYNTRACE

/********************************************************************************
 * dyntracer
 *******************************************************************************/

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer);

#endif

/********************************************************************************
 * application
 *******************************************************************************/

/*  mutator  */
void instrumentr_tracer_set_application(instrumentr_tracer_t tracer,
                                        instrumentr_application_t application);

/*  mutator  */
void instrumentr_tracer_remove_application(instrumentr_tracer_t tracer);

/********************************************************************************
 * status
 *******************************************************************************/

void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status);
int instrumentr_tracer_stack_size(instrumentr_tracer_t tracer);

/********************************************************************************
 * reset
 *******************************************************************************/

void instrumentr_tracer_reset(instrumentr_tracer_t tracer);

/********************************************************************************
 * active_callback
 *******************************************************************************/

/* mutator  */
void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback);

/* mutator  */
void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer);

#endif /* INSTRUMENTR_TRACER_INTERNALS_H */
