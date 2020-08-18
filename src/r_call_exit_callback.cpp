#include "r_call_exit_callback.h"
#include "r_callback.h"
#include "CallExitCallback.hpp"

using instrumentr::CallExitCallback;

SEXP r_call_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<CallExitCallback>(r_function);
}

SEXP r_call_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<CallExitCallback>(r_c_function);
}
