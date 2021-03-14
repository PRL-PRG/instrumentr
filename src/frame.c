#include "frame.h"
#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"
#include "promise.h"
#include "call.h"
#include "context.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_frame_impl_t {
    struct instrumentr_object_impl_t object;
    instrumentr_object_t kernel;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_frame_finalize(instrumentr_object_t object) {
    instrumentr_frame_t frame = (instrumentr_frame_t)(object);

    /* do not kill promise as it is owned by state */
    if(frame->kernel->type == INSTRUMENTR_PROMISE) {
        instrumentr_object_release(frame->kernel);
    }
    else {
        instrumentr_object_kill(frame->kernel);
    }

    frame->kernel = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_frame_t instrumentr_frame_create(instrumentr_state_t state,
                                             instrumentr_object_t kernel) {
    instrumentr_object_t object =
        instrumentr_object_create_and_initialize(sizeof(struct instrumentr_frame_impl_t),
                                                 state,
                                                 INSTRUMENTR_FRAME,
                                                 instrumentr_frame_finalize,
                                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_frame_t frame = (instrumentr_frame_t)(object);

    instrumentr_object_acquire(kernel);
    frame->kernel = kernel;

    return frame;
}

instrumentr_frame_t
instrumentr_frame_create_from_call(instrumentr_state_t state,
                                   instrumentr_call_t call) {
    return instrumentr_frame_create(state, (instrumentr_object_t) call);
}

instrumentr_frame_t
instrumentr_frame_create_from_promise(instrumentr_state_t state,
                                      instrumentr_promise_t promise) {
    return instrumentr_frame_create(state, (instrumentr_object_t) promise);
}

instrumentr_frame_t
instrumentr_frame_create_from_context(instrumentr_state_t state,
                                      instrumentr_context_t context) {
    return instrumentr_frame_create(state, (instrumentr_object_t) context);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_frame_wrap(instrumentr_frame_t frame) {
    return instrumentr_object_wrap((instrumentr_object_t)(frame));
}

instrumentr_frame_t instrumentr_frame_unwrap(SEXP r_frame) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_frame, INSTRUMENTR_FRAME);
    return (instrumentr_frame_t)(object);
}

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_call(instrumentr_frame_t frame) {
    return frame->kernel->type == INSTRUMENTR_CALL;
}

SEXP r_instrumentr_frame_is_call(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_call(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_call_t instrumentr_frame_as_call(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_call(frame)) {
        return (instrumentr_call_t)(frame->kernel);
    } else {
        instrumentr_log_error("frame is not a call");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_frame_as_call(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    instrumentr_call_t call = instrumentr_frame_as_call(frame);
    return instrumentr_call_wrap(call);
}

/********************************************************************************
 * promise
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_promise(instrumentr_frame_t frame) {
    return frame->kernel->type == INSTRUMENTR_PROMISE;
}

SEXP r_instrumentr_frame_is_promise(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_promise(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_promise_t instrumentr_frame_as_promise(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_promise(frame)) {
        return (instrumentr_promise_t)(frame->kernel);
    } else {
        instrumentr_log_error("frame is not a promise");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_frame_as_promise(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    instrumentr_promise_t promise = instrumentr_frame_as_promise(frame);
    return instrumentr_promise_wrap(promise);
}

/********************************************************************************
 * context
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_context(instrumentr_frame_t frame) {
    return frame->kernel->type == INSTRUMENTR_CONTEXT;
}

SEXP r_instrumentr_frame_is_context(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_context(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_context_t instrumentr_frame_as_context(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_context(frame)) {
        return (instrumentr_context_t)(frame->kernel);
    } else {
        instrumentr_log_error("frame is not a context");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_frame_as_context(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    instrumentr_context_t context = instrumentr_frame_as_context(frame);
    return instrumentr_context_wrap(context);
}
