#include <instrumentr/call_stack.h>
#include <instrumentr/memory.h>
#include "object_internals.h"
#include "vec.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_call_t) vec_call_t;

struct instrumentr_call_stack_impl_t {
    instrumentr_object_impl_t object;
    vec_call_t calls;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_call_stack_finalize(instrumentr_object_t object) {
    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(object);

    int size = call_stack -> calls.length;

    for (; size > 0; --size) {
        instrumentr_call_stack_pop(call_stack);
    }

    vec_deinit(&call_stack->calls);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_call_stack_t instrumentr_call_stack_create() {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_call_stack_impl_t),
                                  INSTRUMENTR_CALL_STACK,
                                  instrumentr_call_stack_finalize);

    instrumentr_call_stack_t call_stack = (instrumentr_call_stack_t)(object);

    vec_init(&call_stack->calls);

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
    return call_stack->calls.length;
}

SEXP r_instrumentr_call_stack_get_size(SEXP r_call_stack) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int size = instrumentr_call_stack_get_size(call_stack);
    return instrumentr_c_int_to_r_integer(size);
}

/********************************************************************************
 * calls
 *******************************************************************************/

/* mutator  */
void instrumentr_call_stack_push(instrumentr_call_stack_t call_stack,
                                 instrumentr_call_t call) {
    instrumentr_object_increment_reference(call);
    vec_push(&call_stack->calls, call);
}

/* mutator  */
void instrumentr_call_stack_pop(instrumentr_call_stack_t call_stack) {
    instrumentr_call_t call = vec_pop(&call_stack->calls);
    instrumentr_object_decrement_reference(call);
}

/* accessor  */
instrumentr_call_t
instrumentr_call_stack_peek(instrumentr_call_stack_t call_stack, int index) {
    int reverse_index = instrumentr_call_stack_get_size(call_stack) - 1 - index;
    if (reverse_index < 0) {
        instrumentr_raise_error(
            "attempt to peek beyond the length of call stack");
    } else {
        return call_stack->calls.data[reverse_index];
    }
}

SEXP r_instrumentr_call_stack_peek(SEXP r_call_stack, SEXP r_index) {
    instrumentr_call_stack_t call_stack =
        instrumentr_call_stack_unwrap(r_call_stack);
    int index = instrumentr_r_integer_to_c_int(r_index);
    instrumentr_call_t call = instrumentr_call_stack_peek(call_stack, index);
    return instrumentr_call_wrap(call);
}

#endif /* INSTRUMENTR_CALL_STACK_H */
