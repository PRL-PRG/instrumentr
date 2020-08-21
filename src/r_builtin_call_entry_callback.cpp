#include "r_builtin_call_entry_callback.h"
#include "r_callback.h"
#include "BuiltinCallEntryCallback.hpp"

using instrumentr::BuiltinCallEntryCallback;

SEXP r_builtin_call_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<BuiltinCallEntryCallback>(
        r_function);
}

SEXP r_builtin_call_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<BuiltinCallEntryCallback>(
        r_c_function);
}
