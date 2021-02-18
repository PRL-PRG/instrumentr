#include "r_context_entry_callback.h"
#include "r_callback.h"
#include "ContextEntryCallback.hpp"

using instrumentr::ContextEntryCallback;

SEXP r_context_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ContextEntryCallback>(r_function);
}

SEXP r_context_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ContextEntryCallback>(r_c_function);
}
