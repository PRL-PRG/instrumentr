#include "CallStack.hpp"
#include "Call.hpp"
#include "r_call_stack.h"

using instrumentr::Call;
using instrumentr::CallStack;
using instrumentr::CallStackSPtr;
using instrumentr::from_sexp;
using instrumentr::to_sexp;

SEXP r_call_stack_get_size(SEXP r_call_stack) {
    CallStackSPtr call_stack = from_sexp<CallStack>(r_call_stack);
    return ScalarInteger(call_stack->get_size());
}

SEXP r_call_stack_peek_frame(SEXP r_call_stack, SEXP r_index) {
    CallStackSPtr call_stack = from_sexp<CallStack>(r_call_stack);
    int index = asInteger(r_index);
    /* NOTE: 1 based indexing at the R level */
    return to_sexp<Call>(call_stack->peek_frame(index - 1));
}
