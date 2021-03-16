#include "call_stack.h"
#include "frame.h"
#include "interop.h"
#include "vec.h"
#include "context.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_frame_t) vec_frame_t;

struct instrumentr_call_stack_impl_t {
    struct instrumentr_model_impl_t model;
    vec_frame_t frames;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_stack_finalize(instrumentr_model_t model) {
    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(model);

    int size = call_stack->frames.length;

    for (; size > 0; --size) {
        instrumentr_call_stack_pop_frame(call_stack);
    }

    vec_deinit(&call_stack->frames);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t
instrumentr_call_stack_create(instrumentr_state_t state) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_call_stack_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CALL_STACK,
                                 instrumentr_call_stack_finalize,
                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(model);

    vec_init(&call_stack->frames);

    return call_stack;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(call_stack,
                                     INSTRUMENTR_MODEL_TYPE_CALL_STACK)

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
void instrumentr_call_stack_push_frame(instrumentr_call_stack_t call_stack,
                                       instrumentr_frame_t frame) {
    instrumentr_model_acquire(frame);
    vec_push(&call_stack->frames, frame);
}

/* mutator  */
void instrumentr_call_stack_pop_frame(instrumentr_call_stack_t call_stack) {
    instrumentr_frame_t frame = vec_pop(&call_stack->frames);
    instrumentr_model_kill(frame);
}

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_get_frame(instrumentr_call_stack_t call_stack,
                                 int index) {
    int size = instrumentr_call_stack_get_size(call_stack);
    if (index >= size) {
        instrumentr_log_error(
            "attempt to get frame beyond the size of call stack");
        /* NOTE: not executed */
        return NULL;
    } else {
        return call_stack->frames.data[index];
    }
}

SEXP r_instrumentr_call_stack_get_frame(SEXP r_call_stack, SEXP r_index) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int index = instrumentr_r_integer_to_c_int(r_index);
    instrumentr_frame_t frame =
        instrumentr_call_stack_get_frame(call_stack, index - 1);
    return instrumentr_frame_wrap(frame);
}

/* accessor  */
instrumentr_frame_t
instrumentr_call_stack_peek_frame(instrumentr_call_stack_t call_stack,
                                  int index) {
    int reverse_index = instrumentr_call_stack_get_size(call_stack) - 1 - index;
    return instrumentr_call_stack_get_frame(call_stack, reverse_index);
}

SEXP r_instrumentr_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int index = instrumentr_r_integer_to_c_int(r_index);
    instrumentr_frame_t frame =
        instrumentr_call_stack_peek_frame(call_stack, index - 1);
    return instrumentr_frame_wrap(frame);
}

int instrumentr_call_stack_get_jumped_frame_count(
    instrumentr_call_stack_t call_stack,
    void* pointer) {
    int size = instrumentr_call_stack_get_size(call_stack);

    for (int i = size - 1; i >= 0; --i) {
        instrumentr_frame_t frame =
            instrumentr_call_stack_get_frame(call_stack, i);

        if (instrumentr_frame_is_context(frame)) {
            instrumentr_context_t context = instrumentr_frame_as_context(frame);

            if (instrumentr_context_get_pointer(context) == pointer) {
                return size - i - 1;
            }
        }
    }

    instrumentr_log_error("cannot find matching context for jumping");
    /* NOTE: not executed */
    return -1;
}
