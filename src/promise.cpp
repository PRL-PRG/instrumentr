#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"
#include "argument.h"
#include "parameter.h"
#include "call.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_promise_impl_t {
    struct instrumentr_object_impl_t object;
    SEXP r_promise;
    instrumentr_promise_type_t type;
    instrumentr_argument_t argument;
    instrumentr_parameter_t parameter;
    instrumentr_call_t call;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_promise_finalize(instrumentr_object_t object) {
    instrumentr_promise_t promise = (instrumentr_promise_t)(object);

    promise->r_promise = NULL;

    switch (promise->type) {
    case INSTRUMENTR_PROMISE_TYPE_ARGUMENT:
        instrumentr_object_release(promise->argument);
        instrumentr_object_release(promise->parameter);
        instrumentr_object_release(promise->call);
        break;
    case INSTRUMENTR_PROMISE_TYPE_LAZY_LOAD:
        instrumentr_object_release(promise->call);
        break;

    case INSTRUMENTR_PROMISE_TYPE_DELAYED_ASSIGN:
        instrumentr_object_release(promise->call);
        break;

    case INSTRUMENTR_PROMISE_TYPE_UNKNOWN:
        break;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(SEXP r_promise) {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_promise_impl_t),
                                  INSTRUMENTR_PROMISE,
                                  instrumentr_promise_finalize);

    instrumentr_promise_t promise = (instrumentr_promise_t)(object);

    promise->r_promise = r_promise;

    promise->type = INSTRUMENTR_PROMISE_TYPE_UNKNOWN;
    promise->argument = nullptr;
    promise->parameter = nullptr;
    promise->call = nullptr;

    return promise;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_promise_wrap(instrumentr_promise_t promise) {
    return instrumentr_object_wrap((instrumentr_object_t)(promise));
}

instrumentr_promise_t instrumentr_promise_unwrap(SEXP r_promise) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_promise, INSTRUMENTR_PROMISE);
    return (instrumentr_promise_t)(object);
}

/********************************************************************************
 * r_promise
 *******************************************************************************/

/* accessor  */
int instrumentr_promise_is_forced(instrumentr_promise_t promise) {
    return PRVALUE(promise->r_promise) != R_UnboundValue;
}

SEXP r_instrumentr_promise_is_forced(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    int result = instrumentr_promise_is_forced(promise);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * expression
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_promise_get_expression(instrumentr_promise_t promise) {
    return PREXPR(promise->r_promise);
}

SEXP r_instrumentr_promise_get_expression(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    return instrumentr_promise_get_expression(promise);
}

/********************************************************************************
 * value
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_promise_get_value(instrumentr_promise_t promise) {
    if (instrumentr_promise_is_forced(promise)) {
        return PRVALUE(promise->r_promise);
    } else {
        instrumentr_log_error("promise is not forced");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_promise_get_value(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    return instrumentr_promise_get_value(promise);
}

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_promise_type_t
instrumentr_promise_get_type(instrumentr_promise_t promise) {
    return promise->type;
}

SEXP r_instrumentr_promise_get_type(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    instrumentr_promise_type_t type = instrumentr_promise_get_type(promise);

    const char* type_str = NULL;

    switch (type) {
    case INSTRUMENTR_PROMISE_TYPE_ARGUMENT:
        type_str = "call";
        break;
    case INSTRUMENTR_PROMISE_TYPE_DELAYED_ASSIGN:
        type_str = "delayed-assign";
        break;
    case INSTRUMENTR_PROMISE_TYPE_LAZY_LOAD:
        type_str = "lazy-load";
        break;
    case INSTRUMENTR_PROMISE_TYPE_UNKNOWN:
        type_str = "unknown";
        break;
    }
    return instrumentr_c_string_to_r_character(type_str);
}

/* accessor  */
int instrumentr_promise_is_argument(instrumentr_promise_t promise) {
    return promise->type == INSTRUMENTR_PROMISE_TYPE_ARGUMENT;
}

SEXP r_instrumentr_promise_is_argument(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    int result = instrumentr_promise_is_argument(promise);
    return instrumentr_c_int_to_r_logical(result);
}

/* mutator */
void instrumentr_promise_set_argument(instrumentr_promise_t promise,
                                      instrumentr_call_t call,
                                      instrumentr_parameter_t parameter,
                                      instrumentr_argument_t argument) {
    promise->type = INSTRUMENTR_PROMISE_TYPE_ARGUMENT;

    promise->call = call;
    instrumentr_object_acquire(call);

    promise->parameter = parameter;
    instrumentr_object_acquire(parameter);

    promise->argument = argument;
    instrumentr_object_acquire(argument);
}

/* accessor  */
int instrumentr_promise_is_delayed_assign(instrumentr_promise_t promise) {
    return promise->type == INSTRUMENTR_PROMISE_TYPE_DELAYED_ASSIGN;
}

SEXP r_instrumentr_promise_is_delayed_assign(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    int result = instrumentr_promise_is_delayed_assign(promise);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
int instrumentr_promise_is_lazy_load(instrumentr_promise_t promise) {
    return promise->type == INSTRUMENTR_PROMISE_TYPE_LAZY_LOAD;
}

SEXP r_instrumentr_promise_is_lazy_load(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    int result = instrumentr_promise_is_lazy_load(promise);
    return instrumentr_c_int_to_r_logical(result);
}

/* accessor  */
int instrumentr_promise_is_unknown(instrumentr_promise_t promise) {
    return promise->type == INSTRUMENTR_PROMISE_TYPE_UNKNOWN;
}

SEXP r_instrumentr_promise_is_unknown(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    int result = instrumentr_promise_is_unknown(promise);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * argument
 *******************************************************************************/

/* accessor  */
instrumentr_argument_t
instrumentr_promise_get_argument(instrumentr_promise_t promise) {
    if (promise->type == INSTRUMENTR_PROMISE_TYPE_ARGUMENT) {
        return promise->argument;
    }
    instrumentr_log_error(
        "attempt to get argument from a non argument promise");
    /* NOTE: not executed */
    return NULL;
}

SEXP r_instrumentr_promise_get_argument(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    instrumentr_argument_t argument = instrumentr_promise_get_argument(promise);
    return instrumentr_argument_wrap(argument);
}

/********************************************************************************
 * parameter
 *******************************************************************************/

/* accessor  */
instrumentr_parameter_t
instrumentr_promise_get_parameter(instrumentr_promise_t promise) {
    if (promise->type == INSTRUMENTR_PROMISE_TYPE_ARGUMENT) {
        return promise->parameter;
    }
    instrumentr_log_error(
        "attempt to get parameter from a non parameter promise");
    /* NOTE: not executed */
    return NULL;
}

SEXP r_instrumentr_promise_get_parameter(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    instrumentr_parameter_t parameter =
        instrumentr_promise_get_parameter(promise);
    return instrumentr_parameter_wrap(parameter);
}

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
instrumentr_call_t instrumentr_promise_get_call(instrumentr_promise_t promise) {
    if (promise->type == INSTRUMENTR_PROMISE_TYPE_UNKNOWN) {
        instrumentr_log_error(
            "attempt to get call from a promise with unknown type");
        /* NOTE: not executed */
        return NULL;
    }

    return promise->call;
}

SEXP r_instrumentr_promise_get_call(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    instrumentr_call_t call = instrumentr_promise_get_call(promise);
    return instrumentr_call_wrap(call);
}
