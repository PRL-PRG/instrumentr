#include "promise.h"
#include "interop.h"
#include "utilities.h"
#include "argument.h"
#include "parameter.h"
#include "call.h"
#include <vector>

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_promise_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_promise;
    instrumentr_promise_type_t type;
    std::vector<instrumentr_promise_call_info_t>* call_info_seq;
    instrumentr_call_t call;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_promise_finalize(instrumentr_model_t model) {
    instrumentr_promise_t promise = (instrumentr_promise_t)(model);

    promise->r_promise = NULL;

    switch (promise->type) {
    case INSTRUMENTR_PROMISE_TYPE_ARGUMENT:
        for (int i = 0; i < promise->call_info_seq->size(); ++i) {
            auto& call_info = promise->call_info_seq->at(i);
            instrumentr_model_release(call_info.argument);
            instrumentr_model_release(call_info.parameter);
            instrumentr_model_release(call_info.call);
        }
        delete promise->call_info_seq;
        promise->call_info_seq = NULL;
        break;
    case INSTRUMENTR_PROMISE_TYPE_LAZY_LOAD:
        instrumentr_model_release(promise->call);
        promise->call = NULL;
        break;

    case INSTRUMENTR_PROMISE_TYPE_DELAYED_ASSIGN:
        instrumentr_model_release(promise->call);
        promise->call = NULL;
        break;

    case INSTRUMENTR_PROMISE_TYPE_UNKNOWN:
        break;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(instrumentr_state_t state,
                                                 SEXP r_promise) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_promise_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_PROMISE,
                                 instrumentr_promise_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_promise_t promise = (instrumentr_promise_t)(model);

    promise->r_promise = r_promise;

    promise->type = INSTRUMENTR_PROMISE_TYPE_UNKNOWN;
    promise->call_info_seq = new std::vector<instrumentr_promise_call_info_t>();
    promise->call = nullptr;

    return promise;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(promise, INSTRUMENTR_MODEL_TYPE_PROMISE)

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
void instrumentr_promise_add_call(instrumentr_promise_t promise,
                                  instrumentr_call_t call,
                                  instrumentr_parameter_t parameter,
                                  instrumentr_argument_t argument) {
    /* if promis is already an argument promise then warn and release the
     * current call */
    promise->type = INSTRUMENTR_PROMISE_TYPE_ARGUMENT;

    instrumentr_promise_call_info_t call_info;
    call_info.call = call;
    call_info.parameter = parameter;
    call_info.argument = argument;

    promise->call_info_seq->push_back(call_info);

    instrumentr_model_acquire(call);
    instrumentr_model_acquire(parameter);
    instrumentr_model_acquire(argument);
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
 * call_info_seq
 *******************************************************************************/

const std::vector<instrumentr_promise_call_info_t>&
instrumentr_promise_get_call_info(instrumentr_promise_t promise) {
    return *promise->call_info_seq;
}

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
instrumentr_call_t instrumentr_promise_get_call(instrumentr_promise_t promise) {
    if (promise->type == INSTRUMENTR_PROMISE_TYPE_UNKNOWN ||
        promise->type == INSTRUMENTR_PROMISE_TYPE_ARGUMENT) {
        instrumentr_log_error(
            "attempt to get call from an unknown or argument promise");
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
