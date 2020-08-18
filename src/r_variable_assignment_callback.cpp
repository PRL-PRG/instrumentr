#include "r_variable_assignment_callback.h"
#include "r_callback.h"
#include "VariableAssignmentCallback.hpp"

using instrumentr::VariableAssignmentCallback;

SEXP r_variable_assignment_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<VariableAssignmentCallback>(
        r_function);
}

SEXP r_variable_assignment_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<VariableAssignmentCallback>(
        r_c_function);
}
