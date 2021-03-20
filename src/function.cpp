#include "function.h"
#include "interop.h"
#include "utilities.h"
#include "environment.h"
#include "funtab.h"
#include "state.h"
#include "frame.h"
#include "call.h"
#include "call_stack.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_function_impl_t {
    struct instrumentr_model_impl_t model;
    instrumentr_function_type_t type;
    int funtab_index;
    const char* name;
    int parameter_count;
    instrumentr_environment_t environment;
    instrumentr_function_t parent;

    SEXP r_definition;

    int exported;
    char* generic_name;
    char* object_class;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_function_finalize(instrumentr_model_t model) {
    instrumentr_function_t function = (instrumentr_function_t)(model);

    free((char*) (function->name));
    function->name = NULL;

    instrumentr_model_release(function->environment);
    function->environment = NULL;

    if (function->parent != NULL) {
        instrumentr_model_release(function->parent);
        function->parent = NULL;
    }

    function->r_definition = NULL;

    free((char*) function->generic_name);
    free((char*) function->object_class);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_function_t instrumentr_function_create(instrumentr_state_t state,
                                                   SEXP r_definition) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_function_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_FUNCTION,
                                 instrumentr_function_finalize,
                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_function_t function = (instrumentr_function_t)(model);

    function->r_definition = r_definition;
    function->exported = 0;
    function->generic_name = NULL;
    function->object_class = NULL;

    if (TYPEOF(r_definition) == CLOSXP) {
        function->type = INSTRUMENTR_FUNCTION_CLOSURE;

        function->funtab_index = -1;

        function->name = NULL;

        function->parameter_count = Rf_length(CAR(r_definition));

        SEXP r_environment = CLOENV(r_definition);
        function->environment =
            instrumentr_state_environment_table_lookup(state, r_environment, 1);

        instrumentr_model_acquire(function->environment);

        instrumentr_environment_type_t environment_type =
            instrumentr_environment_get_type(function->environment);

        /* nested function  */
        if (environment_type != INSTRUMENTR_ENVIRONMENT_TYPE_NAMESPACE &&
            environment_type != INSTRUMENTR_ENVIRONMENT_TYPE_PACKAGE) {
            instrumentr_call_stack_t call_stack =
                instrumentr_state_get_call_stack(state);
            int size = instrumentr_call_stack_get_size(call_stack);

            for (int i = size - 1; i >= 0; --i) {
                instrumentr_frame_t frame =
                    instrumentr_call_stack_get_frame(call_stack, i);
                if (!instrumentr_frame_is_call(frame)) {
                    continue;
                }
                instrumentr_call_t call = instrumentr_frame_as_call(frame);
                instrumentr_function_t call_fun =
                    instrumentr_call_get_function(call);

                /* call's environment is function's lexical environment */
                if (instrumentr_function_is_closure(call_fun) &&
                    instrumentr_call_get_environment(call) ==
                        function->environment) {
                    function->parent = call_fun;
                    instrumentr_model_acquire(function->parent);
                    break;
                }
            }
        }
    } else {
        function->type = TYPEOF(r_definition) == SPECIALSXP
                             ? INSTRUMENTR_FUNCTION_SPECIAL
                             : INSTRUMENTR_FUNCTION_BUILTIN;

        function->funtab_index = instrumentr_funtab_get_index(r_definition);

        function->name = instrumentr_duplicate_string(
            instrumentr_funtab_get_name(function->funtab_index));

        function->parameter_count =
            instrumentr_funtab_get_parameter_count(function->funtab_index);

        function->environment = instrumentr_state_environment_table_lookup(
            state, R_BaseNamespace, 1);
        instrumentr_model_acquire(function->environment);

        /* builtins and specials are never nested */
        function->parent = NULL;
    }

    return function;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(function, INSTRUMENTR_MODEL_TYPE_FUNCTION)

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
                                   const char* name) {
    function->name = instrumentr_duplicate_string(name);
}

/********************************************************************************
 * definition
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
 * environment
 *******************************************************************************/

/* accessor  */
instrumentr_environment_t
instrumentr_function_get_environment(instrumentr_function_t function) {
    return function->environment;
}

SEXP r_instrumentr_function_get_environment(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    instrumentr_environment_t environment =
        instrumentr_function_get_environment(function);
    return instrumentr_environment_wrap(environment);
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
instrumentr_function_t
instrumentr_function_get_parent(instrumentr_function_t function) {
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
 * exported
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_exported(instrumentr_function_t function) {
    return function->exported;
}

SEXP r_instrumentr_function_is_exported(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_exported(function);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator */
void instrumentr_function_set_exported(instrumentr_function_t function) {
    function->exported = 1;
}

/********************************************************************************
 * s3_generic
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_s3_generic(instrumentr_function_t function) {
    return function->generic_name == NULL && function->object_class != NULL;
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
    return function->generic_name != NULL;
}

SEXP r_instrumentr_function_is_s3_method(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_s3_method(function);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * object_class
 *******************************************************************************/

/* mutator */
void instrumentr_function_set_object_class(instrumentr_function_t function,
                                           const char* object_class) {
    function->object_class = instrumentr_duplicate_string(object_class);
}

/********************************************************************************
 * generic_name
 *******************************************************************************/

/* mutator */
void instrumentr_function_set_generic_name(instrumentr_function_t function,
                                           const char* generic_name) {
    function->generic_name = instrumentr_duplicate_string(generic_name);
}

/********************************************************************************
 * internal
 *******************************************************************************/

/* accessor  */
int instrumentr_function_is_internal(instrumentr_function_t function) {
    if (function->type == INSTRUMENTR_FUNCTION_CLOSURE)
        return 0;

    return instrumentr_funtab_is_internal(function->funtab_index);
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
    if (function->type == INSTRUMENTR_FUNCTION_CLOSURE)
        return 0;

    return instrumentr_funtab_is_primitive(function->funtab_index);
}

SEXP r_instrumentr_function_is_primitive(SEXP r_function) {
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    int result = instrumentr_function_is_primitive(function);
    return instrumentr_c_int_to_r_logical(result);
}
