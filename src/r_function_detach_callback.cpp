#include "r_function_detach_callback.h"
#include "r_callback.h"
#include "FunctionDetachCallback.hpp"

using instrumentr::FunctionDetachCallback;

SEXP r_function_detach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<FunctionDetachCallback>(r_function);
}

SEXP r_function_detach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<FunctionDetachCallback>(r_c_function);
}
