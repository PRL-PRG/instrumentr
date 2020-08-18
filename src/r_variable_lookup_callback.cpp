#include "r_variable_lookup_callback.h"
#include "r_callback.h"
#include "VariableLookupCallback.hpp"

using instrumentr::VariableLookupCallback;

SEXP r_variable_lookup_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<VariableLookupCallback>(r_function);
}

SEXP r_variable_lookup_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<VariableLookupCallback>(r_c_function);
}
