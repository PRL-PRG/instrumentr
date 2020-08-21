#include "r_special_call_entry_callback.h"
#include "r_callback.h"
#include "SpecialCallEntryCallback.hpp"

using instrumentr::SpecialCallEntryCallback;

SEXP r_special_call_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<SpecialCallEntryCallback>(
        r_function);
}

SEXP r_special_call_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<SpecialCallEntryCallback>(
        r_c_function);
}
