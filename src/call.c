#include "call.h"
#include "parameter.h"
#include "argument.h"
#include "vec.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"
#include "state.h"
#include "promise.h"
#include "value.h"
#include "function.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_parameter_t) instrumentr_parameter_vector_t;

struct instrumentr_call_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_function_t function;
    SEXP r_expression;
    SEXP r_environment;
    int frame_position;
    int active;
    SEXP r_result;
    instrumentr_parameter_vector_t parameters;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_finalize(instrumentr_object_t object) {
    instrumentr_call_t call = (instrumentr_call_t)(object);

    instrumentr_object_release(call->function);

    call->r_expression;
    call->r_environment;
    call->r_result;

    int count = call->parameters.length;
    instrumentr_parameter_t* parameters = call->parameters.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        instrumentr_object_kill(parameter);
        instrumentr_object_release(parameter);
    }

    vec_deinit(&call->parameters);
}

/********************************************************************************
 * create
 *******************************************************************************/

const char* unwrap_name(SEXP r_name) {
    const char* name = NULL;

    if (TYPEOF(r_name) == SYMSXP) {
        name = R_CHAR(PRINTNAME(r_name));
    }

    return name;
}

void process_promise_argument(instrumentr_state_t state,
                              instrumentr_call_t call,
                              instrumentr_parameter_t parameter,
                              SEXP r_argument_name,
                              SEXP r_argument_value) {
    instrumentr_promise_t promise =
        instrumentr_state_promise_table_lookup(state, r_argument_value, 1);

    instrumentr_argument_t argument =
        instrumentr_argument_create_from_promise(state, unwrap_name(r_argument_name), promise);

    instrumentr_parameter_append_argument(parameter, argument);
    /* NOTE: argument is owned by parameter now */
    instrumentr_object_release(argument);

    instrumentr_promise_set_argument(promise, call, parameter, argument);
}


void process_value_argument(instrumentr_state_t state,
                            instrumentr_parameter_t parameter,
                            SEXP r_argument_name,
                            SEXP r_argument_value) {
    instrumentr_value_t value = instrumentr_value_create(state, r_argument_value);

    instrumentr_argument_t argument = instrumentr_argument_create_from_value(state,
                                                                             unwrap_name(r_argument_name), value);
    /* NOTE: value is owned by argument now */
    instrumentr_object_release(value);

    instrumentr_parameter_append_argument(parameter, argument);
    /* NOTE: argument is owned by parameter now */
    instrumentr_object_release(argument);
}

void process_vararg_argument(instrumentr_state_t state,
                             instrumentr_call_t call,
                             instrumentr_parameter_t parameter,
                             SEXP r_argument_value) {
    for (SEXP r_dot_argument_pointer = r_argument_value;
         r_dot_argument_pointer != R_NilValue;
         r_dot_argument_pointer = CDR(r_dot_argument_pointer)) {

        SEXP r_dot_argument_name = TAG(r_dot_argument_pointer);

        SEXP r_dot_argument_value = CAR(r_dot_argument_pointer);

        /* promise value */
        if(TYPEOF(r_dot_argument_value) == PROMSXP) {
            process_promise_argument(state,
                                     call,
                                     parameter,
                                     r_dot_argument_name,
                                     r_dot_argument_value);
        }
        /* non promise value */
        else {
            process_value_argument(state,
                                   parameter,
                                   r_dot_argument_name,
                                   r_dot_argument_value);
        }
    }
}

void process_parameter(instrumentr_state_t state,
                       instrumentr_call_t call,
                       SEXP r_argument_name,
                       SEXP r_argument_value,
                       SEXP r_default_argument,
                       int position) {

    const char* argument_name = unwrap_name(r_argument_name);

    instrumentr_parameter_t parameter = instrumentr_parameter_create(state,
                                                                     argument_name,
                                                                     position,
                                                                     r_default_argument);

    instrumentr_call_append_parameter(call, parameter);

    /* NOTE: parameter owned by call now */
    instrumentr_object_release(parameter);

    /* missing argument */
    if (r_argument_value == R_UnboundValue ||
        r_argument_value == R_MissingArg) {
        /* NOTE: do nothing */
    }
    /* ... */
    else if (TYPEOF(r_argument_value) == DOTSXP) {
        process_vararg_argument(state, call, parameter, r_argument_value);
    }
    /* promise  */
    else if (TYPEOF(r_argument_value) == PROMSXP) {
        process_promise_argument(state, call, parameter, R_NilValue, r_argument_value);
    }
    /* non promise  */
    else {
        process_value_argument(state, parameter, R_NilValue, r_argument_value);
    }
}

void process_closure_parameters(instrumentr_state_t state,
                                instrumentr_call_t call,
                                instrumentr_function_t function,
                                SEXP r_environment) {
    /* TODO - process calls to special and builtin */
    instrumentr_function_definition_t function_definition =
        instrumentr_function_get_definition(function);

    SEXP r_parameter_list = FORMALS(function_definition.sexp);

    for (int position = 0; r_parameter_list != R_NilValue;
         ++position, r_parameter_list = CDR(r_parameter_list)) {
        SEXP r_argument_name = TAG(r_parameter_list);

        SEXP r_default_argument = CAR(r_parameter_list);
        
        SEXP r_argument_value =
            Rf_findVarInFrame(r_environment, r_argument_name);

        process_parameter(state,
                          call,
                         r_argument_name,
                         r_argument_value,
                         r_default_argument,
                         position);
    }
}

void process_non_closure_parameters(instrumentr_state_t state,
                                    instrumentr_call_t call, SEXP r_args) {
    for (int position = 0; r_args != R_NilValue;
         ++position, r_args = CDR(r_args)) {
        SEXP r_argument_name = TAG(r_args);

        // NOTE: no default arguments for non-closures
        SEXP r_default_argument = NULL;

        SEXP r_argument_value = CAR(r_args);

        process_parameter(state,
                          call,
                         r_argument_name,
                         r_argument_value,
                         r_default_argument,
                         position);
    }
}

instrumentr_call_t instrumentr_call_create(instrumentr_state_t state,
                                           instrumentr_function_t function,
                                           SEXP r_expression,
                                           SEXP r_environment,
                                           int frame_position) {
    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_call_impl_t),
                                                 state,
                                                 INSTRUMENTR_CALL,
                                                 instrumentr_call_finalize,
                                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_call_t call = (instrumentr_call_t)(object);

    call->function = function;
    instrumentr_object_acquire(call->function);

    call->r_expression = r_expression;

    call->r_environment = r_environment;

    call->frame_position = frame_position;

    vec_init(&call->parameters);

    if(instrumentr_function_is_closure(function)) {
        process_closure_parameters(state, call, function, r_environment);
    }
    else {
        process_non_closure_parameters(state, call, CDR(r_expression));
    }

    return call;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_call_wrap(instrumentr_call_t call) {
    return instrumentr_object_wrap((instrumentr_object_t)(call));
}

instrumentr_call_t instrumentr_call_unwrap(SEXP r_call) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_call, INSTRUMENTR_CALL);
    return (instrumentr_call_t)(object);
}

/********************************************************************************
 * function
 *******************************************************************************/

/* accessor  */
instrumentr_function_t instrumentr_call_get_function(instrumentr_call_t call) {
    return call->function;
}

SEXP r_instrumentr_call_get_function(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_function_t function = instrumentr_call_get_function(call);
    return instrumentr_function_wrap(function);
}

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_expression(instrumentr_call_t call) {
    return call->r_expression;
}

SEXP r_instrumentr_call_get_expression(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_expression(call);
}

/********************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_call_get_environment(instrumentr_call_t call) {
    return call->r_environment;
}

SEXP r_instrumentr_call_get_environment(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_environment(call);
}

/********************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_frame_position(instrumentr_call_t call) {
    return call->frame_position;
}

SEXP r_instrumentr_call_get_frame_position(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int frame_position = instrumentr_call_get_frame_position(call);
    return instrumentr_c_int_to_r_integer(frame_position);
}

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_call_is_active(instrumentr_call_t call) {
    return call->active;
}

SEXP r_instrumentr_call_is_active(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int active = instrumentr_call_is_active(call);
    return instrumentr_c_int_to_r_logical(active);
}

/* mutator  */
void instrumentr_call_activate(instrumentr_call_t call) {
    call->active = 1;
}

/* mutator  */
void instrumentr_call_deactivate(instrumentr_call_t call) {
    call->active = 0;
}

/********************************************************************************
 * r_result
 *******************************************************************************/

/* accessor  */
int instrumentr_call_has_result(instrumentr_call_t call) {
    return call->r_result != NULL;
}

SEXP r_instrumentr_call_has_result(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int result = instrumentr_call_has_result(call);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
SEXP instrumentr_call_get_result(instrumentr_call_t call) {
    if (instrumentr_call_has_result(call)) {
        return call->r_result;
    } else {
        instrumentr_log_error("call does not have a result");
        /* NOTE: never executed*/
        return NULL;
    }
}

SEXP r_instrumentr_call_get_result(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    return instrumentr_call_get_result(call);
}

/* mutator */
void instrumentr_call_set_result(instrumentr_call_t call, SEXP r_result) {
    call->r_result = r_result;
}

/********************************************************************************
 * parameters
 *******************************************************************************/

/* accessor  */
int instrumentr_call_get_parameter_count(instrumentr_call_t call) {
    return call->parameters.length;
}

SEXP r_instrumentr_call_get_parameter_count(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    int count = instrumentr_call_get_parameter_count(call);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_position(instrumentr_call_t call,
                                           int position) {
    int count = instrumentr_call_get_parameter_count(call);
    if (position < count && position >= 0) {
        return call->parameters.data[position];
    } else {
        instrumentr_log_error(
            "attempt to access %d parameter of a call with %d parameters",
            position,
            count);
        /* NOTE: never executed*/
        return NULL;
    }
}

SEXP r_instrumentr_call_get_parameter_by_position(SEXP r_call,
                                                  SEXP r_position) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_parameter_t parameter =
        instrumentr_call_get_parameter_by_position(call, position);
    return instrumentr_parameter_wrap(parameter);
}

/* accessor  */
instrumentr_parameter_t
instrumentr_call_get_parameter_by_name(instrumentr_call_t call,
                                       const char* name) {
    int count = instrumentr_call_get_parameter_count(call);
    instrumentr_parameter_t* parameters = call->parameters.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        if (strcmp(instrumentr_parameter_get_name(parameter), name) == 0) {
            return parameter;
        }
    }

    instrumentr_log_error(
        "parameter with name '%s' does not exist for this call",
        name);
    /* NOTE: never executed*/
    return NULL;
}

SEXP r_instrumentr_call_get_parameter_by_name(SEXP r_call, SEXP r_name) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_parameter_t parameter =
        instrumentr_call_get_parameter_by_name(call, name);
    return instrumentr_parameter_wrap(parameter);
}

/* accessor  */
SEXP r_instrumentr_call_get_parameters(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_parameter_t* parameters = call->parameters.data;
    int count = instrumentr_call_get_parameter_count(call);

    SEXP r_parameters = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_parameter_t parameter = parameters[i];
        const char* name = instrumentr_parameter_get_name(parameter);
        SET_VECTOR_ELT(r_parameters, i, instrumentr_parameter_wrap(parameter));
        SET_STRING_ELT(r_names, i, mkChar(name));
    }

    Rf_setAttrib(r_parameters, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_parameters;
}

/* mutator  */
void instrumentr_call_append_parameter(instrumentr_call_t call,
                                       instrumentr_parameter_t parameter) {
    vec_push(&call->parameters, parameter);
    instrumentr_object_acquire(parameter);
}
