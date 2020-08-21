#include "r_closure_call_entry_callback.h"
#include "r_callback.h"
#include "ClosureCallEntryCallback.hpp"

using instrumentr::ClosureCallEntryCallback;

SEXP r_closure_call_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ClosureCallEntryCallback>(
        r_function);
}

SEXP r_closure_call_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ClosureCallEntryCallback>(
        r_c_function);
}
