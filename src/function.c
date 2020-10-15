#include <instrumentr/function.h>
#include <instrumentr/object.h>
#include "interop.h"
#include "utilities.h"
#include "object_internals.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_function_impl_t {
    struct instrumentr_object_impl_t object;
    const char* name;
    int parameter_count;
    SEXP r_definition;
    int public;
    int s3_generic;
    int s3_method;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_function_finalize(instrumentr_object_t object) {
    instrumentr_function_t function = (instrumentr_function_t)(object);

    free((char*)(function->name));

    instrumentr_sexp_release(function->r_definition);
    function->r_definition = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t instrumentr_function_create(const char* name,
                                                   int parameter_count,
                                                   SEXP r_definition,
                                                   int public,
                                                   int s3_generic,
                                                   int s3_method) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_function_impl_t),
                                  INSTRUMENTR_FUNCTION,
                                  instrumentr_function_finalize);

    instrumentr_function_t function = (instrumentr_function_t)(object);

    function->name = duplicate_name;
    function->parameter_count = parameter_count;

    instrumentr_sexp_acquire(r_definition);
    function->r_definition = r_definition;

    function->public = public;
    function->s3_generic = s3_generic;
    function->s3_method = s3_method;

    return function;
}

SEXP r_instrumentr_function_create(SEXP r_name,
                                   SEXP r_parameter_count,
                                   SEXP r_definition,
                                   SEXP r_public,
                                   SEXP r_s3_generic,
                                   SEXP r_s3_method) {
    const char* name = instrumentr_r_character_to_c_string(r_name);
    int parameter_count = instrumentr_r_integer_to_c_int(r_parameter_count);
    int public = instrumentr_r_logical_to_c_int(r_public);
    int s3_generic = instrumentr_r_logical_to_c_int(r_s3_generic);
    int s3_method = instrumentr_r_logical_to_c_int(r_s3_method);
    instrumentr_function_t function = instrumentr_function_create(
        name, parameter_count, r_definition, public, s3_generic, s3_method);
    return instrumentr_function_wrap(function);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_function_wrap(instrumentr_function_t function) {
    return instrumentr_object_wrap((instrumentr_object_t)(function));
}

instrumentr_function_t instrumentr_function_unwrap(SEXP r_function) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_function, INSTRUMENTR_FUNCTION);
    return (instrumentr_function_t)(object);
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_function_get_name(instrumentr_function_t function) {
    return function->name;
}

SEXP r_instrumentr_function_get_name(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    const char* name = instrumentr_function_get_name(function);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * r_definition
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_function_get_definition(instrumentr_function_t function) {
    return function->r_definition;
}

SEXP r_instrumentr_function_get_definition(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    return instrumentr_function_get_definition(function);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_function_get_parameter_count(instrumentr_function_t function) {
    return function->parameter_count;
}

SEXP r_instrumentr_function_get_parameter_count(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_get_parameter_count(function);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * public
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_public(instrumentr_function_t function) {
    return function->public;
}

SEXP r_instrumentr_function_is_public(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_public(function);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * s3_generic
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_s3_generic(instrumentr_function_t function) {
    return function->s3_generic;
}

SEXP r_instrumentr_function_is_s3_generic(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_s3_generic(function);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * s3_method
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_s3_method(instrumentr_function_t function) {
    return function->s3_method;
}

SEXP r_instrumentr_function_is_s3_method(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_s3_method(function);
    return instrumentr_c_int_to_r_logical(result);
}
