#include "r_closure_call_exit_callback.h"
#include "r_callback.h"
#include "ClosureCallExitCallback.hpp"

using instrumentr::ClosureCallExitCallback;

SEXP r_closure_call_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ClosureCallExitCallback>(
        r_function);
}

SEXP r_closure_call_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ClosureCallExitCallback>(
        r_c_function);
}
