#include "r_call_entry_callback.h"
#include "r_callback.h"
#include "CallEntryCallback.hpp"

using instrumentr::CallEntryCallback;

SEXP r_call_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<CallEntryCallback>(r_function);
}

SEXP r_call_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<CallEntryCallback>(r_c_function);
}
