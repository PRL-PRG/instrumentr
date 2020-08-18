#include "r_variable_definition_callback.h"
#include "r_callback.h"
#include "VariableDefinitionCallback.hpp"

using instrumentr::VariableDefinitionCallback;

SEXP r_variable_definition_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<VariableDefinitionCallback>(
        r_function);
}

SEXP r_variable_definition_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<VariableDefinitionCallback>(
        r_c_function);
}
