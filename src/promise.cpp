#include "call.h"
#include "promise.h"
#include "interop.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_promise_impl_t {
    struct instrumentr_value_impl_t value;
    instrumentr_promise_type_t type;
    std::vector<instrumentr_call_t>* calls;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_promise_finalize(instrumentr_value_t value) {
    instrumentr_promise_t promise = (instrumentr_promise_t)(value);

    if (promise->type != INSTRUMENTR_PROMISE_TYPE_UNKNOWN) {
        for (std::size_t i = 0; i < promise->calls->size(); ++i) {
            instrumentr_call_release(promise->calls->at(i));
        }
        delete promise->calls;
        promise->calls = NULL;
    }
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_promise_t instrumentr_promise_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_promise_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_PROMISE,
                                 instrumentr_promise_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_promise_t promise = (instrumentr_promise_t)(value);

    promise->type = INSTRUMENTR_PROMISE_TYPE_UNKNOWN;
    promise->calls = new std::vector<instrumentr_call_t>();

    return promise;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_PROMISE, promise, promise)

/********************************************************************************
 * r_promise
 *******************************************************************************/

/* accessor  */
int instrumentr_promise_is_forced(instrumentr_promise_t promise) {
    SEXP r_sexp = instrumentr_promise_get_sexp(promise);
    return PRVALUE(r_sexp) != R_UnboundValue;
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
    SEXP r_sexp = instrumentr_promise_get_sexp(promise);
    return PREXPR(r_sexp);
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
        return PRVALUE(instrumentr_promise_get_sexp(promise));
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
                                  instrumentr_call_t call) {
    /* if promis is already an argument promise then warn and release the
     * current call */
    promise->type = INSTRUMENTR_PROMISE_TYPE_ARGUMENT;

    promise->calls->push_back(call);

    instrumentr_call_acquire(call);
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
 * calls
 *******************************************************************************/

const std::vector<instrumentr_call_t>&
instrumentr_promise_get_calls(instrumentr_promise_t promise) {
    return *promise->calls;
}

/* accessor  */
instrumentr_call_t instrumentr_promise_get_call(instrumentr_promise_t promise) {
    if (promise->type == INSTRUMENTR_PROMISE_TYPE_UNKNOWN ||
        promise->type == INSTRUMENTR_PROMISE_TYPE_ARGUMENT) {
        instrumentr_log_error(
            "attempt to get call from an unknown or argument promise");
        /* NOTE: not executed */
        return NULL;
    }

    return promise->calls->at(0);
}

SEXP r_instrumentr_promise_get_call(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    instrumentr_call_t call = instrumentr_promise_get_call(promise);
    return instrumentr_call_wrap(call);
}
