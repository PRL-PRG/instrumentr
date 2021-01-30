#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_promise_impl_t {
    struct instrumentr_object_impl_t object;
    SEXP r_promise;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_promise_finalize(instrumentr_object_t object) {
    instrumentr_promise_t promise = (instrumentr_promise_t)(object);

    instrumentr_sexp_release(promise->r_promise);
    promise->r_promise = NULL;
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

    instrumentr_sexp_acquire(r_promise);
    promise->r_promise = r_promise;

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

/* accessor  */
SEXP instrumentr_promise_get_expression(instrumentr_promise_t promise) {
    return PREXPR(promise->r_promise);
}

SEXP r_instrumentr_promise_get_expression(SEXP r_promise) {
    instrumentr_promise_t promise = instrumentr_promise_unwrap(r_promise);
    return instrumentr_promise_get_expression(promise);
}

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
