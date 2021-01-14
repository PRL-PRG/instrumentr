#include "function.h"
#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_function_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_function_type_t type;
    int funtab_index;
    const char* name;
    int parameter_count;
    instrumentr_function_definition_t definition;
    int pub;
    int s3_generic;
    int s3_method;
    int internal;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_function_finalize(instrumentr_object_t object) {
    instrumentr_function_t function = (instrumentr_function_t)(object);

    free((char*) (function->name));

    if (function->type == INSTRUMENTR_FUNCTION_CLOSURE) {
        instrumentr_sexp_release(function->definition.sexp);
    }

    function->definition.ccode = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t
instrumentr_function_create(instrumentr_function_type_t type,
                            int funtab_index,
                            const char* name,
                            int parameter_count,
                            instrumentr_function_definition_t definition,
                            int pub,
                            int s3_generic,
                            int s3_method,
                            int internal) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_function_impl_t),
                                  INSTRUMENTR_FUNCTION,
                                  instrumentr_function_finalize);

    instrumentr_function_t function = (instrumentr_function_t)(object);

    function->type = type;
    function->funtab_index = funtab_index;

    function->name = duplicate_name;
    function->parameter_count = parameter_count;

    if (type == INSTRUMENTR_FUNCTION_CLOSURE) {
        instrumentr_sexp_acquire(definition.sexp);
    }

    function->definition = definition;

    function->pub = pub;
    function->s3_generic = s3_generic;
    function->s3_method = s3_method;
    function->internal = internal;

    return function;
}

instrumentr_function_t instrumentr_function_create_builtin(int funtab_index,
                                                           const char* name,
                                                           int parameter_count,
                                                           CCODE ccode,
                                                           int internal) {
    instrumentr_function_definition_t definition;
    definition.ccode = ccode;

    return instrumentr_function_create(INSTRUMENTR_FUNCTION_BUILTIN,
                                       funtab_index,
                                       name,
                                       parameter_count,
                                       definition,
                                       0,
                                       0,
                                       0,
                                       internal);
}

instrumentr_function_t instrumentr_function_create_special(int funtab_index,
                                                           const char* name,
                                                           int parameter_count,
                                                           CCODE ccode,
                                                           int internal) {
    instrumentr_function_definition_t definition;
    definition.ccode = ccode;

    return instrumentr_function_create(INSTRUMENTR_FUNCTION_SPECIAL,
                                       funtab_index,
                                       name,
                                       parameter_count,
                                       definition,
                                       0,
                                       0,
                                       0,
                                       internal);
}

instrumentr_function_t instrumentr_function_create_closure(const char* name,
                                                           int parameter_count,
                                                           SEXP sexp,
                                                           int pub,
                                                           int s3_generic,
                                                           int s3_method) {
    instrumentr_function_definition_t definition;
    definition.sexp = sexp;

    return instrumentr_function_create(INSTRUMENTR_FUNCTION_CLOSURE,
                                       -1,
                                       name,
                                       parameter_count,
                                       definition,
                                       pub,
                                       s3_generic,
                                       s3_method,
                                       0);
}

SEXP r_instrumentr_function_create_closure(SEXP r_name,
                                           SEXP r_parameter_count,
                                           SEXP r_definition,
                                           SEXP r_pub,
                                           SEXP r_s3_generic,
                                           SEXP r_s3_method) {
    const char* name = instrumentr_r_character_to_c_string(r_name);
    int parameter_count = instrumentr_r_integer_to_c_int(r_parameter_count);
    int pub = instrumentr_r_logical_to_c_int(r_pub);
    int s3_generic = instrumentr_r_logical_to_c_int(r_s3_generic);
    int s3_method = instrumentr_r_logical_to_c_int(r_s3_method);
    instrumentr_function_t function = instrumentr_function_create_closure(
        name, parameter_count, r_definition, pub, s3_generic, s3_method);
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
 * type
 *******************************************************************************/

instrumentr_function_type_t
instrumentr_function_get_type(instrumentr_function_t function) {
    return function->type;
}

int instrumentr_function_is_builtin(instrumentr_function_t function) {
    return function->type == INSTRUMENTR_FUNCTION_BUILTIN;
}

SEXP r_instrumentr_function_is_builtin(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_builtin(function);
    return instrumentr_c_int_to_r_logical(result);
}

int instrumentr_function_is_special(instrumentr_function_t function) {
    return function->type == INSTRUMENTR_FUNCTION_SPECIAL;
}

SEXP r_instrumentr_function_is_special(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_special(function);
    return instrumentr_c_int_to_r_logical(result);
}

int instrumentr_function_is_closure(instrumentr_function_t function) {
    return function->type == INSTRUMENTR_FUNCTION_CLOSURE;
}

SEXP r_instrumentr_function_is_closure(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_closure(function);
    return instrumentr_c_int_to_r_logical(result);
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
 * definition
 *******************************************************************************/

/* accessor  */
instrumentr_function_definition_t
instrumentr_function_get_definition(instrumentr_function_t function) {
    return function->definition;
}

SEXP r_instrumentr_function_get_definition(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    instrumentr_function_definition_t definition =
        instrumentr_function_get_definition(function);

    if (instrumentr_function_is_closure(function) ==
        INSTRUMENTR_FUNCTION_CLOSURE) {
        return definition.sexp;
    } else {
        return instrumentr_c_pointer_to_r_externalptr(
            definition.ccode, R_NilValue, R_NilValue, NULL);
    }
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
    return function->pub;
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

/********************************************************************************
 * internal
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_internal(instrumentr_function_t function) {
    return (function->type != INSTRUMENTR_FUNCTION_CLOSURE) &&
           function->internal;
}

SEXP r_instrumentr_function_is_internal(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_internal(function);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * primitive
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_primitive(instrumentr_function_t function) {
    return (function->type != INSTRUMENTR_FUNCTION_CLOSURE) &&
           (!function->internal);
}

SEXP r_instrumentr_function_is_primitive(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_primitive(function);
    return instrumentr_c_int_to_r_logical(result);
}
