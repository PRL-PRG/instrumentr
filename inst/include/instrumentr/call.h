#ifndef INSTRUMENTR_CALL_H
#define INSTRUMENTR_CALL_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/parameter.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_call_impl_t* instrumentr_call_t;

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_call_wrap(instrumentr_call_t call);
instrumentr_call_t instrumentr_call_unwrap(SEXP r_call);

/********************************************************************************
 * function
 *******************************************************************************/

/* accessor  */
instrumentr_function_t instrumentr_call_get_function(instrumentr_call_t call);
SEXP r_instrumentr_call_get_function(SEXP r_call);

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_expression(instrumentr_call_t call);
SEXP r_instrumentr_call_get_expression(SEXP r_call);

/********************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_environment(instrumentr_call_t call);
SEXP r_instrumentr_call_get_environment(SEXP r_call);

/********************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_frame_position(instrumentr_call_t call);
SEXP r_instrumentr_call_get_frame_position(SEXP r_call);

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_call_is_active(instrumentr_call_t call);
SEXP r_instrumentr_call_is_active(SEXP r_call);

/********************************************************************************
 * result
 *******************************************************************************/

/* accessor  */
int instrumentr_call_has_result(instrumentr_call_t call);
SEXP r_instrumentr_call_has_result(SEXP r_call);

/* accessor  */
SEXP instrumentr_call_get_result(instrumentr_call_t call);
SEXP r_instrumentr_call_get_result(SEXP r_call);

/********************************************************************************
 * parameters
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_parameter_count(instrumentr_call_t call);
SEXP r_instrumentr_call_get_parameter_count(SEXP r_call);

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_position(instrumentr_call_t call,
                                           int position);
SEXP r_instrumentr_call_get_parameter_by_position(SEXP r_call, SEXP r_position);

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_name(instrumentr_call_t call,
                                       const char* name);
SEXP r_instrumentr_call_get_parameter_by_name(SEXP r_call, SEXP r_name);

/* accessor  */
SEXP r_instrumentr_call_get_parameters(SEXP r_call);

#endif /* INSTRUMENTR_CALL_H */
