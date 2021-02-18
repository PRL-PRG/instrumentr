#include "r_context_exit_callback.h"
#include "r_callback.h"
#include "ContextExitCallback.hpp"

using instrumentr::ContextExitCallback;

SEXP r_context_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ContextExitCallback>(r_function);
}

SEXP r_context_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ContextExitCallback>(r_c_function);
}
