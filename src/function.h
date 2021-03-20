#ifndef INSTRUMENTR_FUNCTION_H
#define INSTRUMENTR_FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "model.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t instrumentr_function_create(instrumentr_state_t state,
                                                   SEXP r_function);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(function, INSTRUMENTR_MODEL_TYPE_FUNCTION)

/********************************************************************************
 * type
 *******************************************************************************/

instrumentr_function_type_t
instrumentr_function_get_type(instrumentr_function_t function);

int instrumentr_function_is_builtin(instrumentr_function_t function);
SEXP r_instrumentr_function_is_builtin(SEXP r_function);

int instrumentr_function_is_special(instrumentr_function_t function);
SEXP r_instrumentr_function_is_special(SEXP r_function);

int instrumentr_function_is_closure(instrumentr_function_t function);
SEXP r_instrumentr_function_is_closure(SEXP r_function);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_function_get_name(instrumentr_function_t function);
SEXP r_instrumentr_function_get_name(SEXP r_function);

int instrumentr_function_has_name(instrumentr_function_t function);
SEXP r_instrumentr_function_has_name(SEXP r_function);

/* mutator */
void instrumentr_function_set_name(instrumentr_function_t function,
                                   const char* name);

/********************************************************************************
 * definition
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_function_get_definition(instrumentr_function_t function);
SEXP r_instrumentr_function_get_definition(SEXP r_function);

/********************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
instrumentr_environment_t
instrumentr_function_get_environment(instrumentr_function_t function);
SEXP r_instrumentr_function_get_environment(SEXP r_function);

/********************************************************************************
 * parent
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_inner(instrumentr_function_t function);

SEXP r_instrumentr_function_is_inner(SEXP r_function);

/* accessor  */
instrumentr_function_t
instrumentr_function_get_parent(instrumentr_function_t function);

SEXP r_instrumentr_function_get_parent(SEXP r_function);

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_function_get_parameter_count(instrumentr_function_t function);
SEXP r_instrumentr_function_get_parameter_count(SEXP r_function);

/********************************************************************************
 * exported
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_exported(instrumentr_function_t function);
SEXP r_instrumentr_function_is_exported(SEXP r_function);
/* mutator */
void instrumentr_function_set_exported(instrumentr_function_t function);

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

/********************************************************************************
 * object_class
 *******************************************************************************/

/* mutator */
void instrumentr_function_set_object_class(instrumentr_function_t function,
                                           const char* object_class);

/********************************************************************************
 * generic_name
 *******************************************************************************/

/* mutator */
void instrumentr_function_set_generic_name(instrumentr_function_t function,
                                           const char* generic_name);

/********************************************************************************
 * internal
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_internal(instrumentr_function_t function);
SEXP r_instrumentr_function_is_internal(SEXP r_function);

/********************************************************************************
 * primitive
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_primitive(instrumentr_function_t function);
SEXP r_instrumentr_function_is_primitive(SEXP r_function);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FUNCTION_H */
