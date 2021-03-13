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
    instrumentr_function_t parent;
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
    function->name = NULL;

    function->definition.ccode = NULL;

    if(function->parent != NULL) {
        instrumentr_object_release(function->parent);
        function->parent = NULL;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t
instrumentr_function_create(instrumentr_state_t state,
                            instrumentr_function_type_t type,
                            int funtab_index,
                            const char* name,
                            int parameter_count,
                            instrumentr_function_definition_t definition,
                            instrumentr_function_t parent,
                            int pub,
                            int s3_generic,
                            int s3_method,
                            int internal) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_function_impl_t),
                                                 state,
                                                 INSTRUMENTR_FUNCTION,
                                                 instrumentr_function_finalize,
                                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_function_t function = (instrumentr_function_t)(object);

    function->type = type;
    function->funtab_index = funtab_index;

    function->name = duplicate_name;
    function->parameter_count = parameter_count;

    /* NOTE: definition sexp is not acquired */
    function->definition = definition;

    function->parent = parent;
    if(parent != NULL) {
        instrumentr_object_acquire(parent);
    }

    function->pub = pub;
    function->s3_generic = s3_generic;
    function->s3_method = s3_method;
    function->internal = internal;

    return function;
}

instrumentr_function_t instrumentr_function_create_builtin(instrumentr_state_t state,
                                                           int funtab_index,
                                                           const char* name,
                                                           int parameter_count,
                                                           CCODE ccode,
                                                           int internal) {
    instrumentr_function_definition_t definition;
    definition.ccode = ccode;

    return instrumentr_function_create(state,
                                       INSTRUMENTR_FUNCTION_BUILTIN,
                                       funtab_index,
                                       name,
                                       parameter_count,
                                       definition,
                                       NULL,
                                       0,
                                       0,
                                       0,
                                       internal);
}

instrumentr_function_t instrumentr_function_create_special(instrumentr_state_t state,
                                                           int funtab_index,
                                                           const char* name,
                                                           int parameter_count,
                                                           CCODE ccode,
                                                           int internal) {
    instrumentr_function_definition_t definition;
    definition.ccode = ccode;

    return instrumentr_function_create(state,
                                       INSTRUMENTR_FUNCTION_SPECIAL,
                                       funtab_index,
                                       name,
                                       parameter_count,
                                       definition,
                                       NULL,
                                       0,
                                       0,
                                       0,
                                       internal);
}

instrumentr_function_t
instrumentr_function_create_closure(instrumentr_state_t state,
                                    const char* name,
                                    int parameter_count,
                                    SEXP sexp,
                                    instrumentr_function_t parent,
                                    int pub,
                                    int s3_generic,
                                    int s3_method) {
    instrumentr_function_definition_t definition;
    definition.sexp = sexp;

    return instrumentr_function_create(state,
                                       INSTRUMENTR_FUNCTION_CLOSURE,
                                       -1,
                                       name,
                                       parameter_count,
                                       definition,
                                       parent,
                                       pub,
                                       s3_generic,
                                       s3_method,
                                       0);
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

int instrumentr_function_has_name(instrumentr_function_t function) {
    return function->name != NULL;
}

SEXP r_instrumentr_function_has_name(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_has_name(function);
    return instrumentr_c_int_to_r_logical(result);
}


/* mutator */
void instrumentr_function_set_name(instrumentr_function_t function,
                                   const char* name){
    function->name = instrumentr_duplicate_string(name);
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
 * parent
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_inner(instrumentr_function_t function) {
    return function->parent != NULL;
}

SEXP r_instrumentr_function_is_inner(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_inner(function);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_function_t instrumentr_function_get_parent(instrumentr_function_t function) {
    return function->parent;
}

SEXP r_instrumentr_function_get_parent(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    instrumentr_function_t parent = instrumentr_function_get_parent(function);
    return instrumentr_function_wrap(parent);
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

/* mutator */
void instrumentr_function_set_public(instrumentr_function_t function,
                                     int pub) {
    function->pub = pub;
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

/* mutator */
void instrumentr_function_set_s3_generic(instrumentr_function_t function, int s3_generic) {
    function->s3_generic = s3_generic;
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

/* mutator */
void instrumentr_function_set_s3_method(instrumentr_function_t function,
                                         int s3_method) {
    function->s3_method = s3_method;
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
