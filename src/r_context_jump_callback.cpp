#include "r_context_jump_callback.h"
#include "r_callback.h"
#include "ContextJumpCallback.hpp"

using instrumentr::ContextJumpCallback;

SEXP r_context_jump_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ContextJumpCallback>(r_function);
}

SEXP r_context_jump_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ContextJumpCallback>(r_c_function);
}
