#include "../inst/include/CallStack.hpp"
#include "../inst/include/Call.hpp"
#include "r_api.h"

using lightr::Call;
using lightr::CallStack;
using lightr::CallStackSPtr;

SEXP r_call_stack_get_size(SEXP r_call_stack) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    return ScalarInteger(call_stack->get_size());
}

SEXP r_call_stack_get_frame(SEXP r_call_stack, SEXP r_index) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    int index = asInteger(r_index);
    /* NOTE: 1 based indexing at the R level */
    return Call::to_sexp(call_stack->get_frame(index - 1));
}

SEXP r_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index) {
    CallStackSPtr call_stack = CallStack::from_sexp(r_call_stack);
    int index = asInteger(r_index);
    return Call::to_sexp(call_stack->peek_frame(index));
}

