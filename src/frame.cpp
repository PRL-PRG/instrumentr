#include "frame.h"
#include "interop.h"
#include "utilities.h"
#include "promise.h"
#include "call.h"
#include "context.h"
#include "eval.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_frame_impl_t {
    struct instrumentr_model_impl_t model;
    instrumentr_model_t kernel;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_frame_finalize(instrumentr_model_t model) {
    instrumentr_frame_t frame = (instrumentr_frame_t) (model);

    /* do not kill promise as it is owned by state */
    switch (instrumentr_model_get_type(frame->kernel)) {
    case INSTRUMENTR_MODEL_TYPE_EVAL:
        instrumentr_model_kill(frame->kernel);
        break;

    case INSTRUMENTR_MODEL_TYPE_VALUE:
        /* this means it is a promise */
        instrumentr_model_release(frame->kernel);
        break;

    case INSTRUMENTR_MODEL_TYPE_CALL:
        instrumentr_model_kill(frame->kernel);
        break;

    case INSTRUMENTR_MODEL_TYPE_CONTEXT:
        instrumentr_model_kill(frame->kernel);
        break;

    default:
        instrumentr_log_error("unsupported model type on frame");
        break;
    }

    frame->kernel = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_frame_t instrumentr_frame_create(instrumentr_state_t state,
                                             instrumentr_model_t kernel) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_frame_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_FRAME,
                                 instrumentr_frame_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_frame_t frame = (instrumentr_frame_t) (model);

    frame->kernel = kernel;
    instrumentr_model_acquire(frame->kernel);
    return frame;
}

instrumentr_frame_t
instrumentr_frame_create_from_eval(instrumentr_state_t state,
                                   instrumentr_eval_t eval) {
    return instrumentr_frame_create(state, (instrumentr_model_t) eval);
}

instrumentr_frame_t
instrumentr_frame_create_from_call(instrumentr_state_t state,
                                   instrumentr_call_t call) {
    return instrumentr_frame_create(state, (instrumentr_model_t) call);
}

instrumentr_frame_t
instrumentr_frame_create_from_promise(instrumentr_state_t state,
                                      instrumentr_promise_t promise) {
    return instrumentr_frame_create(state, (instrumentr_model_t) promise);
}

instrumentr_frame_t
instrumentr_frame_create_from_context(instrumentr_state_t state,
                                      instrumentr_context_t context) {
    return instrumentr_frame_create(state, (instrumentr_model_t) context);
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_DEFINE_DERIVED_API(INSTRUMENTR_MODEL_TYPE_FRAME, frame, frame)

/********************************************************************************
 * eval
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_eval(instrumentr_frame_t frame) {
    return instrumentr_model_get_type(frame->kernel) ==
           INSTRUMENTR_MODEL_TYPE_EVAL;
}

SEXP r_instrumentr_frame_is_eval(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_eval(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_eval_t instrumentr_frame_as_eval(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_eval(frame)) {
        return (instrumentr_eval_t) (frame->kernel);
    } else {
        instrumentr_log_error("frame is not a eval");
        /* NOTE: not executed */
        return NULL;
    }
}

SEXP r_instrumentr_frame_as_eval(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    instrumentr_eval_t eval = instrumentr_frame_as_eval(frame);
    return instrumentr_eval_wrap(eval);
}

/********************************************************************************
 * call
 *******************************************************************************/

/* accessor  */
int instrumentr_frame_is_call(instrumentr_frame_t frame) {
    return instrumentr_model_get_type(frame->kernel) ==
           INSTRUMENTR_MODEL_TYPE_CALL;
}

SEXP r_instrumentr_frame_is_call(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_call(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_call_t instrumentr_frame_as_call(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_call(frame)) {
        return (instrumentr_call_t) (frame->kernel);
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
    return instrumentr_model_get_type(frame->kernel) ==
           INSTRUMENTR_MODEL_TYPE_VALUE;
}

SEXP r_instrumentr_frame_is_promise(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_promise(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_promise_t instrumentr_frame_as_promise(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_promise(frame)) {
        return (instrumentr_promise_t) (frame->kernel);
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
    return instrumentr_model_get_type(frame->kernel) ==
           INSTRUMENTR_MODEL_TYPE_CONTEXT;
}

SEXP r_instrumentr_frame_is_context(SEXP r_frame) {
    instrumentr_frame_t frame = instrumentr_frame_unwrap(r_frame);
    int result = instrumentr_frame_is_context(frame);
    return instrumentr_c_int_to_r_logical(result);
}

instrumentr_context_t instrumentr_frame_as_context(instrumentr_frame_t frame) {
    if (instrumentr_frame_is_context(frame)) {
        return (instrumentr_context_t) (frame->kernel);
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
