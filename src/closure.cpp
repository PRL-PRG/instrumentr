#include "closure.h"
#include "interop.h"
#include "utilities.h"
#include "environment.h"
#include "funtab.h"
#include "state.h"
#include "frame.h"
#include "call.h"
#include "call_stack.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_closure_impl_t {
    struct instrumentr_model_impl_t model;

    SEXP r_sexp;
    const char* name;
    int parameter_count;
    instrumentr_environment_t environment;
    instrumentr_closure_t parent;
    int exported;
    char* generic_name;
    char* object_class;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_closure_finalize(instrumentr_model_t model) {
    instrumentr_closure_t closure = (instrumentr_closure_t)(model);

    closure->r_sexp = NULL;

    free((char*) (closure->name));
    closure->name = NULL;

    if (closure->environment != NULL) {
        instrumentr_model_release(closure->environment);
        closure->environment = NULL;
    }

    if (closure->parent != NULL) {
        instrumentr_model_release(closure->parent);
        closure->parent = NULL;
    }

    closure->exported = -1;
    free((char*) closure->generic_name);
    free((char*) closure->object_class);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_closure_t instrumentr_closure_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_closure_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CLOSURE,
                                 instrumentr_closure_finalize,
                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_closure_t closure = (instrumentr_closure_t)(model);

    closure->r_sexp = r_sexp;
    closure->exported = 0;
    closure->generic_name = NULL;
    closure->object_class = NULL;

    closure->name = NULL;

    closure->parameter_count = Rf_length(FORMALS(r_sexp));

    SEXP r_environment = CLOENV(r_sexp);

    if (TYPEOF(r_environment) == ENVSXP) {
        instrumentr_closure_set_environment(closure);
    }

    return closure;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(closure, INSTRUMENTR_MODEL_TYPE_CLOSURE)

/********************************************************************************
 * sexp
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_closure_get_sexp(instrumentr_closure_t closure) {
    return closure->r_sexp;
}

SEXP r_instrumentr_closure_get_sexp(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    return instrumentr_closure_get_sexp(closure);
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_closure_get_name(instrumentr_closure_t closure) {
    return closure->name;
}

SEXP r_instrumentr_closure_get_name(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    const char* name = instrumentr_closure_get_name(closure);
    return instrumentr_c_string_to_r_character(name);
}

int instrumentr_closure_has_name(instrumentr_closure_t closure) {
    return closure->name != NULL;
}

SEXP r_instrumentr_closure_has_name(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_has_name(closure);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator */
void instrumentr_closure_set_name(instrumentr_closure_t closure,
                                  const char* name) {
    closure->name = instrumentr_duplicate_string(name);
}

/********************************************************************************
 * environment
 *******************************************************************************/
void instrumentr_closure_set_environment(instrumentr_closure_t closure) {
    instrumentr_state_t state = instrumentr_model_get_state(closure);

    SEXP r_environment = CLOENV(closure->r_sexp);

    closure->environment = instrumentr_state_value_table_lookup_environment(
        state, r_environment, 1);

    instrumentr_model_acquire(closure->environment);

    instrumentr_environment_type_t environment_type =
        instrumentr_environment_get_type(closure->environment);

    /* nested closure  */
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
            instrumentr_value_t call_fun = instrumentr_call_get_function(call);

            /* call's environment is closure's lexical environment */
            if (instrumentr_value_is_closure(call_fun) &&
                instrumentr_call_get_environment(call) ==
                    closure->environment) {
                closure->parent = instrumentr_value_as_closure(call_fun);
                instrumentr_model_acquire(closure->parent);
                break;
            }
        }
    }
}

/* accessor  */
instrumentr_environment_t
instrumentr_closure_get_environment(instrumentr_closure_t closure) {
    if (closure->environment == NULL) {
        instrumentr_closure_set_environment(closure);
    }

    return closure->environment;
}

SEXP r_instrumentr_closure_get_environment(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    instrumentr_environment_t environment =
        instrumentr_closure_get_environment(closure);
    return instrumentr_environment_wrap(environment);
}

/********************************************************************************
 * parent
 *******************************************************************************/

/* accessor  */
int instrumentr_closure_is_inner(instrumentr_closure_t closure) {
    return closure->parent != NULL;
}

SEXP r_instrumentr_closure_is_inner(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_is_inner(closure);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
instrumentr_closure_t
instrumentr_closure_get_parent(instrumentr_closure_t closure) {
    return closure->parent;
}

SEXP r_instrumentr_closure_get_parent(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    instrumentr_closure_t parent = instrumentr_closure_get_parent(closure);
    return instrumentr_closure_wrap(parent);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_closure_get_parameter_count(instrumentr_closure_t closure) {
    return closure->parameter_count;
}

SEXP r_instrumentr_closure_get_parameter_count(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_get_parameter_count(closure);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * exported
 *******************************************************************************/

/* accessor  */
int instrumentr_closure_is_exported(instrumentr_closure_t closure) {
    return closure->exported;
}

SEXP r_instrumentr_closure_is_exported(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_is_exported(closure);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator */
void instrumentr_closure_set_exported(instrumentr_closure_t closure) {
    closure->exported = 1;
}

/********************************************************************************
 * s3_generic
 *******************************************************************************/

/* accessor  */
int instrumentr_closure_is_s3_generic(instrumentr_closure_t closure) {
    return closure->generic_name == NULL && closure->object_class != NULL;
}

SEXP r_instrumentr_closure_is_s3_generic(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_is_s3_generic(closure);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * s3_method
 *******************************************************************************/

/* accessor  */
int instrumentr_closure_is_s3_method(instrumentr_closure_t closure) {
    return closure->generic_name != NULL;
}

SEXP r_instrumentr_closure_is_s3_method(SEXP r_closure) {
    instrumentr_closure_t closure = instrumentr_closure_unwrap(r_closure);
    int result = instrumentr_closure_is_s3_method(closure);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * object_class
 *******************************************************************************/

/* mutator */
void instrumentr_closure_set_object_class(instrumentr_closure_t closure,
                                          const char* object_class) {
    closure->object_class = instrumentr_duplicate_string(object_class);
}

/********************************************************************************
 * generic_name
 *******************************************************************************/

/* mutator */
void instrumentr_closure_set_generic_name(instrumentr_closure_t closure,
                                          const char* generic_name) {
    closure->generic_name = instrumentr_duplicate_string(generic_name);
}
