#ifndef INSTRUMENTR_TRACER_INTERNALS_H
#define INSTRUMENTR_TRACER_INTERNALS_H

#include <instrumentr/tracer.h>

/********************************************************************************
 * status
 *******************************************************************************/

void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status);

/********************************************************************************
 * active_callback
 *******************************************************************************/

/* mutator  */
void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback);

/* mutator  */
void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer);

#endif /* INSTRUMENTR_TRACER_INTERNALS_H */
