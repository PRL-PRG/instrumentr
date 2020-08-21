#include "r_special_call_exit_callback.h"
#include "r_callback.h"
#include "SpecialCallExitCallback.hpp"

using instrumentr::SpecialCallExitCallback;

SEXP r_special_call_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<SpecialCallExitCallback>(
        r_function);
}

SEXP r_special_call_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<SpecialCallExitCallback>(
        r_c_function);
}
