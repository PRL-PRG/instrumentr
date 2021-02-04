#include "call_stack.h"
#include "frame.h"
#include "interop.h"
#include "object.h"
#include "vec.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_frame_t) vec_frame_t;

struct instrumentr_call_stack_impl_t {
    struct instrumentr_object_impl_t object;
    vec_frame_t frames;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_stack_finalize(instrumentr_object_t object) {
    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(object);

    int size = call_stack -> frames.length;

    for (; size > 0; --size) {
        instrumentr_call_stack_pop(call_stack);
    }

    vec_deinit(&call_stack->frames);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t instrumentr_call_stack_create() {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_call_stack_impl_t),
                                  INSTRUMENTR_CALL_STACK,
                                  instrumentr_call_stack_finalize);

    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(object);

    vec_init(&call_stack->frames);

    return call_stack;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_call_stack_wrap(instrumentr_call_stack_t call_stack) {
    return instrumentr_object_wrap((instrumentr_object_t)(call_stack));
}

instrumentr_call_stack_t instrumentr_call_stack_unwrap(SEXP r_call_stack) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_call_stack, INSTRUMENTR_CALL_STACK);
    return (instrumentr_call_stack_t)(object);
}

/********************************************************************************
 * size
 *******************************************************************************/

/* accessor  */
int instrumentr_call_stack_get_size(instrumentr_call_stack_t call_stack) {
    return call_stack->frames.length;
}

SEXP r_instrumentr_call_stack_get_size(SEXP r_call_stack) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int size = instrumentr_call_stack_get_size(call_stack);
    return instrumentr_c_int_to_r_integer(size);
}

/********************************************************************************
 * frames
 *******************************************************************************/

/* mutator  */
void instrumentr_call_stack_push(instrumentr_call_stack_t call_stack,
                                 instrumentr_frame_t frame) {
    instrumentr_object_acquire(frame);
    vec_push(&call_stack->frames, frame);
}

/* mutator  */
void instrumentr_call_stack_pop(instrumentr_call_stack_t call_stack) {
    instrumentr_frame_t frame = vec_pop(&call_stack->frames);
    instrumentr_object_release(frame);
}

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_peek_frame(instrumentr_call_stack_t call_stack, int index) {
    int reverse_index = instrumentr_call_stack_get_size(call_stack) - 1 - index;
    if (reverse_index < 0) {
        instrumentr_log_error(
            "attempt to peek beyond the length of call stack");
        /* NOTE: not executed */
        return NULL;
    } else {
        return call_stack->frames.data[reverse_index];
    }
}

SEXP r_instrumentr_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int index = instrumentr_r_integer_to_c_int(r_index);
    instrumentr_frame_t frame = instrumentr_call_stack_peek_frame(call_stack, index);
    return instrumentr_frame_wrap(frame);
}
