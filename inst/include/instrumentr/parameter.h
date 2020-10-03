#ifndef INSTRUMENTR_PARAMETER_H
#define INSTRUMENTR_PARAMETER_H

#include <instrumentr/Rincludes.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_parameter_impl_t* instrumentr_parameter_t;

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(const char* name,
                                                     int position);

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
 * arguments
 *******************************************************************************/

/* accessor  */
argument_vector_t
instrumentr_parameter_get_arguments(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_get_arguments(SEXP r_parameter);

/* accessor  */
int instrumentr_parameter_is_missing(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_is_missing(SEXP r_parameter);

/* accessor  */
int instrumentr_parameter_is_vararg(instrumentr_parameter_t parameter);
SEXP r_instrumentr_parameter_is_vararg(SEXP r_parameter);

#endif /* INSTRUMENTR_PARAMETER_H */
