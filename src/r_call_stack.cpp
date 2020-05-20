#include "../inst/include/CallStack.hpp"
#include "../inst/include/Call.hpp"
#include "r_api.hpp"

SEXP r_call_stack_get_size(SEXP r_call_stack) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    return ScalarInteger(call_stack->get_size());
}

SEXP r_call_stack_get_frame(SEXP r_call_stack, SEXP r_index) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    int index = asInteger(r_index);
    return Call::to_sexp(call_stack->get_frame(index));
}

SEXP r_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    int index = asInteger(r_index);
    return Call::to_sexp(call_stack->peek_frame(index));
}

SEXP r_call_stack_get_call_stack() {
    return CallStack::to_sexp(get_call_stack());
}
