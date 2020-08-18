#include "r_variable_removal_callback.h"
#include "r_callback.h"
#include "VariableRemovalCallback.hpp"

using instrumentr::VariableRemovalCallback;

SEXP r_variable_removal_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<VariableRemovalCallback>(
        r_function);
}

SEXP r_variable_removal_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<VariableRemovalCallback>(
        r_c_function);
}
