#ifndef INSTRUMENTR_PARAMETER_H
#define INSTRUMENTR_PARAMETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(instrumentr_state_t state,
                                                     const char* name,
                                                     int position,
                                                     SEXP r_default_expression);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_parameter_wrap(instrumentr_parameter_t parameter);
instrumentr_parameter_t instrumentr_parameter_unwrap(SEXP r_parameter);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_parameter_get_name(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_get_name(SEXP r_parameter);

/********************************************************************************
 * position
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_get_position(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_get_position(SEXP r_parameter);

/********************************************************************************
 * r_default_argument
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_has_default_argument(
    instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_has_default_argument(SEXP r_parameter);

/* accessor  */
SEXP instrumentr_parameter_get_default_argument(
    instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_get_default_argument(SEXP r_parameter);

/********************************************************************************
 * arguments
 *******************************************************************************/

/* accessor  */
int instrumentr_parameter_get_argument_count(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_get_argument_count(SEXP r_parameter);

/* accessor  */
int instrumentr_parameter_is_missing(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_is_missing(SEXP r_parameter);

/* accessor  */
int instrumentr_parameter_is_vararg(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_is_vararg(SEXP r_parameter);

/* accessor  */
instrumentr_argument_t instrumentr_parameter_get_argument_by_position(
    instrumentr_parameter_t parameter,
    int position);
SEXP r_instrumentr_parameter_get_argument_by_position(SEXP r_parameter,
                                                      SEXP r_position);

/* accessor  */
instrumentr_argument_t
instrumentr_parameter_get_argument_by_name(instrumentr_parameter_t parameter,
                                           const char* name);
SEXP r_instrumentr_parameter_get_argument_by_name(SEXP r_parameter,
                                                  SEXP r_name);

/* accessor */
SEXP r_instrumentr_parameter_get_arguments(SEXP r_parameter);

/* mutator */
void instrumentr_parameter_append_argument(instrumentr_parameter_t parameter,
                                           instrumentr_argument_t argument);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_PARAMETER_H */
