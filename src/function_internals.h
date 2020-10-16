#ifndef INSTRUMENTR_FUNCTION_INTERNALS_H
#define INSTRUMENTR_FUNCTION_INTERNALS_H

#include <instrumentr/Rincludes.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t instrumentr_function_create(const char* name,
                                                   int parameter_count,
                                                   SEXP r_definition,
                                                   int pub,
                                                   int s3_generic,
                                                   int s3_method);

SEXP r_instrumentr_function_create(SEXP r_name,
                                   SEXP r_parameter_count,
                                   SEXP r_definition,
                                   SEXP r_public,
                                   SEXP r_s3_generic,
                                   SEXP r_s3_method);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_function_wrap(instrumentr_function_t function);

instrumentr_function_t instrumentr_function_unwrap(SEXP r_function);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_function_get_name(instrumentr_function_t function);
SEXP r_instrumentr_function_get_name(SEXP r_function);

/********************************************************************************
 * r_definition
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_function_get_definition(instrumentr_function_t function);
SEXP r_instrumentr_function_get_definition(SEXP r_function);

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_function_get_parameter_count(instrumentr_function_t function);
SEXP r_instrumentr_function_get_parameter_count(SEXP r_function);

/********************************************************************************
 * public
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_public(instrumentr_function_t function);
SEXP r_instrumentr_function_is_public(SEXP r_function);

/********************************************************************************
 * s3_generic
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_s3_generic(instrumentr_function_t function);
SEXP r_instrumentr_function_is_s3_generic(SEXP r_function);

/********************************************************************************
 * s3_method
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_s3_method(instrumentr_function_t function);
SEXP r_instrumentr_function_is_s3_method(SEXP r_function);

#endif /* INSTRUMENTR_FUNCTION_INTERNALS_H */
