#ifndef INSTRUMENTR_CALL_INTERNALS_H
#define INSTRUMENTR_CALL_INTERNALS_H

#include <instrumentr/call.h>

/********************************************************************************
 * create
 *******************************************************************************/

SEXP r_instrumentr_call_create(SEXP r_function,
                               SEXP r_expression,
                               SEXP r_environment,
                               SEXP r_frame_position);

/********************************************************************************
 * active
 *******************************************************************************/

/* mutator  */
void instrumentr_call_activate(instrumentr_call_t call);

/* mutator  */
void instrumentr_call_deactivate(instrumentr_call_t call);


/********************************************************************************
 * r_result
 *******************************************************************************/

/* mutator */
void instrumentr_call_set_result(instrumentr_call_t call, SEXP r_result);

/********************************************************************************
 * parameters
 *******************************************************************************/

/* mutator  */
void instrumentr_call_append_parameter(instrumentr_call_t call,
                                       instrumentr_parameter_t parameter);


#endif /* INSTRUMENTR_CALL_INTERNALS_H */
