#include "r_builtin_call_exit_callback.h"
#include "r_callback.h"
#include "BuiltinCallExitCallback.hpp"

using instrumentr::BuiltinCallExitCallback;

SEXP r_builtin_call_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<BuiltinCallExitCallback>(
        r_function);
}

SEXP r_builtin_call_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<BuiltinCallExitCallback>(
        r_c_function);
}
