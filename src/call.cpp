#include "call.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "value.h"
#include "environment.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_call_impl_t {
    struct instrumentr_model_impl_t model;
    instrumentr_value_t function;
    SEXP r_expression;
    instrumentr_environment_t environment;
    int active;
    SEXP r_result;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_finalize(instrumentr_model_t model) {
    instrumentr_call_t call = (instrumentr_call_t)(model);

    instrumentr_value_release(call->function);
    call->function = NULL;

    call->r_expression = NULL;

    instrumentr_environment_release(call->environment);
    call->environment = NULL;

    call->r_result = NULL;
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

// void process_promise_argument(instrumentr_state_t state,
//                              instrumentr_call_t call,
//                              instrumentr_parameter_t parameter,
//                              SEXP r_argument_name,
//                              SEXP r_argument_value) {
//    instrumentr_promise_t promise =
//        instrumentr_state_value_table_lookup_promise(state, r_argument_value,
//        1);
//
//    instrumentr_argument_t argument =
//    instrumentr_argument_create_from_promise(
//        state, unwrap_name(r_argument_name), promise);
//
//    instrumentr_parameter_append_argument(parameter, argument);
//    /* NOTE: argument is owned by parameter now */
//    instrumentr_model_release(argument);
//
//    instrumentr_promise_add_call(promise, call, parameter, argument);
//}
//
// void process_value_argument(instrumentr_state_t state,
//                            instrumentr_parameter_t parameter,
//                            SEXP r_argument_name,
//                            SEXP r_argument_value) {
//    instrumentr_value_t value =
//        instrumentr_value_create(state, r_argument_value);
//
//    instrumentr_argument_t argument = instrumentr_argument_create_from_value(
//        state, unwrap_name(r_argument_name), value);
//    /* NOTE: value is owned by argument now */
//    instrumentr_model_release(value);
//
//    instrumentr_parameter_append_argument(parameter, argument);
//    /* NOTE: argument is owned by parameter now */
//    instrumentr_model_release(argument);
//}
//
// void process_vararg_argument(instrumentr_state_t state,
//                             instrumentr_call_t call,
//                             instrumentr_parameter_t parameter,
//                             SEXP r_argument_value) {
//    for (SEXP r_dot_argument_pointer = r_argument_value;
//         r_dot_argument_pointer != R_NilValue;
//         r_dot_argument_pointer = CDR(r_dot_argument_pointer)) {
//        SEXP r_dot_argument_name = TAG(r_dot_argument_pointer);
//
//        SEXP r_dot_argument_value = CAR(r_dot_argument_pointer);
//
//        /* promise value */
//        if (TYPEOF(r_dot_argument_value) == PROMSXP) {
//            process_promise_argument(state,
//                                     call,
//                                     parameter,
//                                     r_dot_argument_name,
//                                     r_dot_argument_value);
//        }
//        /* non promise value */
//        else {
//            process_value_argument(
//                state, parameter, r_dot_argument_name, r_dot_argument_value);
//        }
//    }
//}
//
// void process_parameter(instrumentr_state_t state,
//                       instrumentr_call_t call,
//                       SEXP r_argument_name,
//                       SEXP r_argument_value,
//                       SEXP r_default_argument,
//                       int position) {
//    const char* argument_name = unwrap_name(r_argument_name);
//
//    instrumentr_parameter_t parameter = instrumentr_parameter_create(
//        state, argument_name, position, r_default_argument);
//
//    instrumentr_call_append_parameter(call, parameter);
//
//    /* NOTE: parameter owned by call now */
//    instrumentr_model_release(parameter);
//
//    /* missing argument */
//    if (r_argument_value == R_UnboundValue ||
//        r_argument_value == R_MissingArg) {
//        /* NOTE: do nothing */
//    }
//    /* ... */
//    else if (TYPEOF(r_argument_value) == DOTSXP) {
//        process_vararg_argument(state, call, parameter, r_argument_value);
//    }
//    /* promise  */
//    else if (TYPEOF(r_argument_value) == PROMSXP) {
//        process_promise_argument(
//            state, call, parameter, R_NilValue, r_argument_value);
//    }
//    /* non promise  */
//    else {
//        process_value_argument(state, parameter, R_NilValue,
//        r_argument_value);
//    }
//}
//
// void process_closure_parameters(instrumentr_state_t state,
//                                instrumentr_call_t call,
//                                instrumentr_function_t function,
//                                SEXP r_environment) {
//    /* TODO - process calls to special and builtin */
//    SEXP r_definition = instrumentr_function_get_definition(function);
//
//    SEXP r_parameter_list = FORMALS(r_definition);
//
//    for (int position = 0; r_parameter_list != R_NilValue;
//         ++position, r_parameter_list = CDR(r_parameter_list)) {
//        SEXP r_argument_name = TAG(r_parameter_list);
//
//        SEXP r_default_argument = CAR(r_parameter_list);
//
//        SEXP r_argument_value =
//            Rf_findVarInFrame(r_environment, r_argument_name);
//
//        process_parameter(state,
//                          call,
//                          r_argument_name,
//                          r_argument_value,
//                          r_default_argument,
//                          position);
//    }
//}
//
// void process_non_closure_parameters(instrumentr_state_t state,
//                                    instrumentr_call_t call,
//                                    SEXP r_args) {
//    for (int position = 0; r_args != R_NilValue;
//         ++position, r_args = CDR(r_args)) {
//        SEXP r_argument_name = TAG(r_args);
//
//        // NOTE: no default arguments for non-closures
//        SEXP r_default_argument = NULL;
//
//        SEXP r_argument_value = CAR(r_args);
//
//        process_parameter(state,
//                          call,
//                          r_argument_name,
//                          r_argument_value,
//                          r_default_argument,
//                          position);
//    }
//}

instrumentr_call_t instrumentr_call_create(instrumentr_state_t state,
                                           instrumentr_value_t function,
                                           SEXP r_expression,
                                           SEXP r_environment) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_call_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CALL,
                                 instrumentr_call_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_call_t call = (instrumentr_call_t)(model);

    call->function = function;
    instrumentr_value_acquire(call->function);

    call->r_expression = r_expression;

    call->environment = instrumentr_state_value_table_lookup_environment(
        state, r_environment, 1);

    instrumentr_environment_acquire(call->environment);

    return call;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(call, INSTRUMENTR_MODEL_TYPE_CALL)

/********************************************************************************
 * function
 *******************************************************************************/

/* accessor  */
instrumentr_value_t instrumentr_call_get_function(instrumentr_call_t call) {
    return call->function;
}

SEXP r_instrumentr_call_get_function(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_value_t function = instrumentr_call_get_function(call);
    return instrumentr_value_wrap(function);
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
instrumentr_environment_t
instrumentr_call_get_environment(instrumentr_call_t call) {
    return call->environment;
}

SEXP r_instrumentr_call_get_environment(SEXP r_call) {
    instrumentr_call_t call = instrumentr_call_unwrap(r_call);
    instrumentr_environment_t environment =
        instrumentr_call_get_environment(call);
    return instrumentr_environment_wrap(environment);
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
